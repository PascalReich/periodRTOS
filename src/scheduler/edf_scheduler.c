/**
 * @file edf_sched.c
 * @brief Earliest Deadline First (EDF) scheduler implementation
 */

#include "periodRTOS.h"
#include <string.h>
#include <limits.h> // Required for UINT32_MAX

/* External variables */
extern TaskControlBlock_t xTaskList[MAX_TASKS];
extern TaskHandle_t xIdleTask;
extern SystemMonitor_t xSystemMonitor;

/* Scheduler state */
// --- EDF CHANGE ---
// The pxReadyList is removed as it's based on static priorities, which EDF does not use.
// static TaskHandle_t pxReadyList[MAX_PRIORITY_LEVELS]; 
static TaskHandle_t pxCurrentTaskTCB = NULL;
uint32_t ulSystemTick = 0;
static bool bSchedulerInitialized = false;

/* External function prototypes */
extern void vTriggerContextSwitch(void);
extern TaskHandle_t pxGetCurrentTask(void);
extern void vSetCurrentTask(TaskHandle_t xTask);
extern void vStartContextSwitch(void);


/* Internal function prototypes */
// --- EDF CHANGE ---
// These functions are either removed or their purpose has changed.
// static void vUpdateTaskPriorities(void); // RM-specific, removed.
static TaskHandle_t pxGetEarliestDeadlineTask(void); // Replaces pxGetHighestPriorityReadyTask
// static void vAddTaskToReadyList(TaskHandle_t xTask); // No-op now
// static void vRemoveTaskFromReadyList(TaskHandle_t xTask); // No-op now
static bool bIsTaskReady(TaskHandle_t xTask);
static void vCheckDeadlines(void);
static void vUpdateTaskTiming(TaskHandle_t xTask);


#if ENABLE_STACK_CANARY
extern uint32_t * ulCanaryAddresses[MAX_TASKS];
#endif


/**
 * @brief Initialize the EDF scheduler
 */
void vSchedulerInit(void)
{
    // --- EDF CHANGE ---
    // The scheduler no longer calculates static priorities or populates a ready list at startup.
    // It only needs to mark itself as initialized.
    bSchedulerInitialized = true;
}

/**
 * @brief Get the next task to run (EDF scheduling)
 */
TaskHandle_t vScheduleNextTask(void)
{
    TaskHandle_t xNextTask;
    
    if (!bSchedulerInitialized) {
        return NULL;
    }
    
    // --- EDF CHANGE ---
    // Get the ready task with the earliest deadline.
    xNextTask = pxGetEarliestDeadlineTask();
    
    /* If no ready task, return idle task */
    if (xNextTask == NULL) {
        xNextTask = xIdleTask;
    }
    
    /* Update current task */
    pxCurrentTaskTCB = xNextTask;
    
    return xNextTask;
}

/**
 * @brief Get the ready task with the earliest absolute deadline.
 * This is the core logic of the EDF scheduler.
 */
static TaskHandle_t pxGetEarliestDeadlineTask(void)
{
    TaskHandle_t pxTaskWithEarliestDeadline = NULL;
    uint32_t ulEarliestDeadline = UINT32_MAX;

    // Iterate through the master task list to find the best candidate.
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        TaskControlBlock_t *pxTCB = &xTaskList[i];

        // Consider only valid, ready tasks.
        if (pxTCB->ulTaskID != 0 && pxTCB->eCurrentState == TASK_STATE_READY) {
            // If this task's deadline is sooner than the earliest we've found so far...
            if (pxTCB->ulDeadlineTime < ulEarliestDeadline) {
                // ...it becomes our new candidate.
                ulEarliestDeadline = pxTCB->ulDeadlineTime;
                pxTaskWithEarliestDeadline = (TaskHandle_t)pxTCB;
            }
        }
    }

    return pxTaskWithEarliestDeadline;
}

/**
 * @brief Add task to ready list (No-op for EDF)
 * @note In this EDF implementation, a task's readiness is determined
 * solely by its eCurrentState. There is no separate ready list to manage.
 */
static void vAddTaskToReadyList(TaskHandle_t xTask)
{
    // This function is intentionally left empty.
    (void)xTask; // Prevent compiler warnings for unused parameter.
}

/**
 * @brief Remove task from ready list (No-op for EDF)
 */
void vRemoveTaskFromReadyList(TaskHandle_t xTask)
{
    // This function is intentionally left empty.
    (void)xTask; // Prevent compiler warnings for unused parameter.
}

/**
 * @brief Check if task is ready
 */
static bool bIsTaskReady(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (xTask == NULL) {
        return false;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    
    return (pxTCB->eCurrentState == TASK_STATE_READY);
}

/**
 * @brief Check task deadlines
 */
static void vCheckDeadlines(void)
{
    TaskControlBlock_t *pxTCB;
    
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        pxTCB = &xTaskList[i];
        
        // --- NOTE --- This logic assumes ulDeadlineTime is correctly updated for every period.
        if (pxTCB->ulTaskID != 0 && pxTCB->eCurrentState == TASK_STATE_RUNNING) {
            /* Check if deadline is missed */
            if (ulSystemTick > pxTCB->ulDeadlineTime) {
                pxTCB->ulDeadlineMissCount++;
                pxTCB->bDeadlineMissed = true;
            }
        }
    }
}

/**
 * @brief Update task timing information
 */
static void vUpdateTaskTiming(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (xTask == NULL) {
        return;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    
    /* Update release and deadline times for periodic tasks */
    if (pxTCB->ulPeriod > 0) {
        pxTCB->ulReleaseTime += pxTCB->ulPeriod;
        // This assumes ulDeadline is the relative deadline for the period.
        pxTCB->ulDeadlineTime = pxTCB->ulReleaseTime + pxTCB->ulDeadline;
    }
}

/**
 * @brief System tick handler - called every 1ms
 */
void vSystemTickHandler(void)
{

    #if ENABLE_STACK_CANARY
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        uint32_t* value = ulCanaryAddresses[i];
        if (value && *value != STACK_CANARY) {
            while(1) {;}
        }
    }
    #endif

    TaskControlBlock_t *pxTCB;
    
    /* Increment system tick */
    ulSystemTick++;
    xSystemMonitor.ulSystemUptime = ulSystemTick;
    
    /* Check deadlines */
    vCheckDeadlines();
    
    /* Check for task releases */
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        pxTCB = &xTaskList[i];
        
        if (pxTCB->ulTaskID != 0 && pxTCB->ulPeriod > 0) {
            /* Check if task should be released */
            if (ulSystemTick >= pxTCB->ulReleaseTime) {
                if (pxTCB->eCurrentState == TASK_STATE_BLOCKED) {
                    pxTCB->eCurrentState = TASK_STATE_READY;
                    // --- EDF CHANGE --- No longer need to add to a ready list.
                    // vAddTaskToReadyList((TaskHandle_t)pxTCB); 
                }
                
                /* Update timing for next release */
                vUpdateTaskTiming((TaskHandle_t)pxTCB);
            }
        }
    }
    
    /* --- EDF CHANGE: Preemption Logic --- */
    // Trigger context switch if a different task now has an earlier deadline.
    TaskHandle_t xCurrentTask = pxGetCurrentTask();
    TaskHandle_t xNextTask = pxGetEarliestDeadlineTask();
    
    if (xNextTask != NULL && xNextTask != xCurrentTask) {
        // If there's a current task running...
        if (xCurrentTask != xIdleTask && xCurrentTask != NULL) {
            TaskControlBlock_t *pxCurrentTCB = (TaskControlBlock_t *)xCurrentTask;
            TaskControlBlock_t *pxNextTCB = (TaskControlBlock_t *)xNextTask;
            
            // Preempt if the new task's deadline is earlier than the current task's deadline.
            if (pxNextTCB->ulDeadlineTime < pxCurrentTCB->ulDeadlineTime) {
                pxCurrentTCB->eCurrentState = TASK_STATE_READY;
                vStartContextSwitch();
            }
        } else {
            // If the current task is idle or null, and a real task is ready, switch immediately.
             vStartContextSwitch();
        }
    }
}

