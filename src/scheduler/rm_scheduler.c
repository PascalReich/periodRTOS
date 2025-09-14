/**
 * @file rm_scheduler.c
 * @brief Rate Monotonic scheduler implementation
 */

#include "periodRTOS.h"
#include <string.h>

/* External variables */
extern TaskControlBlock_t xTaskList[MAX_TASKS];
extern TaskHandle_t xIdleTask;

/* Scheduler state */
static TaskHandle_t pxReadyList[MAX_PRIORITY_LEVELS];
static TaskHandle_t pxCurrentTaskTCB = NULL;
uint32_t ulSystemTick = 0;
static bool bSchedulerInitialized = false;

/* External function prototypes */
extern void vTriggerContextSwitch(void);
extern TaskHandle_t pxGetCurrentTask(void);
extern void vSetCurrentTask(TaskHandle_t xTask);

/* Internal function prototypes */
static void vUpdateTaskPriorities(void);
static TaskHandle_t pxGetHighestPriorityReadyTask(void);
static void vAddTaskToReadyList(TaskHandle_t xTask);
static void vRemoveTaskFromReadyList(TaskHandle_t xTask);
static bool bIsTaskReady(TaskHandle_t xTask);
static void vCheckDeadlines(void);
static void vUpdateTaskTiming(TaskHandle_t xTask);

/**
 * @brief Initialize the Rate Monotonic scheduler
 */
void vSchedulerInit(void)
{
    /* Clear ready list */
    memset(pxReadyList, 0, sizeof(pxReadyList));
    
    /* Update task priorities based on periods (Rate Monotonic) */
    vUpdateTaskPriorities();
    
    /* Add all ready tasks to ready list */
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        TaskControlBlock_t *pxTCB = &xTaskList[i];
        if (pxTCB->ulTaskID != 0 && pxTCB->eCurrentState == TASK_STATE_READY) {
            vAddTaskToReadyList((TaskHandle_t)pxTCB);
        }
    }
    
    bSchedulerInitialized = true;
}

/**
 * @brief Get the next task to run (Rate Monotonic scheduling)
 */
TaskHandle_t vSchedulerGetNextTask(void)
{
    TaskHandle_t xNextTask;
    
    if (!bSchedulerInitialized) {
        return NULL;
    }
    
    /* Get highest priority ready task */
    xNextTask = pxGetHighestPriorityReadyTask();
    
    /* If no ready task, return idle task */
    if (xNextTask == NULL) {
        xNextTask = xIdleTask;
    }
    
    /* Update current task */
    pxCurrentTaskTCB = xNextTask;
    
    /* Update task state */
    if (xNextTask != xIdleTask) {
        TaskControlBlock_t *pxTCB = (TaskControlBlock_t *)xNextTask;
        pxTCB->eCurrentState = TASK_STATE_RUNNING;
        pxTCB->ulLastStartTime = ulSystemTick;
    }
    
    return xNextTask;
}

/**
 * @brief Update task priorities based on Rate Monotonic algorithm
 * Shorter period = higher priority (lower priority number)
 */
static void vUpdateTaskPriorities(void)
{
    TaskControlBlock_t *pxTCB;
    uint32_t ulPriority = 0;
    
    /* Sort tasks by period (ascending order) */
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        for (uint32_t j = i + 1; j < MAX_TASKS; j++) {
            TaskControlBlock_t *pxTCB1 = &xTaskList[i];
            TaskControlBlock_t *pxTCB2 = &xTaskList[j];
            
            if (pxTCB1->ulTaskID != 0 && pxTCB2->ulTaskID != 0) {
                if (pxTCB1->ulPeriod > pxTCB2->ulPeriod) {
                    /* Swap tasks */
                    TaskControlBlock_t xTemp = *pxTCB1;
                    *pxTCB1 = *pxTCB2;
                    *pxTCB2 = xTemp;
                }
            }
        }
    }
    
    /* Assign priorities based on sorted order */
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        pxTCB = &xTaskList[i];
        if (pxTCB->ulTaskID != 0) {
            pxTCB->ulPriority = ulPriority++;
            if (ulPriority >= MAX_PRIORITY_LEVELS) {
                ulPriority = MAX_PRIORITY_LEVELS - 1;
            }
        }
    }
}

/**
 * @brief Get highest priority ready task
 */
static TaskHandle_t pxGetHighestPriorityReadyTask(void)
{
    for (uint32_t i = 0; i < MAX_PRIORITY_LEVELS; i++) {
        if (pxReadyList[i] != NULL) {
            return pxReadyList[i];
        }
    }
    return NULL;
}

/**
 * @brief Add task to ready list based on priority
 */
static void vAddTaskToReadyList(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (xTask == NULL) {
        return;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    
    /* Check if task is already in ready list */
    if (pxReadyList[pxTCB->ulPriority] == xTask) {
        return;
    }
    
    /* Add task to appropriate priority level */
    pxReadyList[pxTCB->ulPriority] = xTask;
}

/**
 * @brief Remove task from ready list
 */
static void vRemoveTaskFromReadyList(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (xTask == NULL) {
        return;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    
    /* Remove task from ready list */
    if (pxReadyList[pxTCB->ulPriority] == xTask) {
        pxReadyList[pxTCB->ulPriority] = NULL;
    }
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
    
    /* Update execution time */
    if (pxTCB->ulLastStartTime > 0) {
        uint32_t ulExecutionTime = ulSystemTick - pxTCB->ulLastStartTime;
        pxTCB->ulExecutionTime += ulExecutionTime;
    }
    
    /* Update release and deadline times for periodic tasks */
    if (pxTCB->ulPeriod > 0) {
        pxTCB->ulReleaseTime = ulSystemTick + pxTCB->ulPeriod;
        pxTCB->ulDeadlineTime = pxTCB->ulReleaseTime + pxTCB->ulDeadline;
    }
}

/**
 * @brief System tick handler - called every 1ms
 */
void vSystemTickHandler(void)
{
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
                    vAddTaskToReadyList((TaskHandle_t)pxTCB);
                }
                
                /* Update timing for next release */
                vUpdateTaskTiming((TaskHandle_t)pxTCB);
            }
        }
    }
    
    /* Trigger context switch if higher priority task is ready */
    TaskHandle_t xCurrentTask = pxGetCurrentTask();
    TaskHandle_t xNextTask = pxGetHighestPriorityReadyTask();
    
    if (xNextTask != NULL && xNextTask != xCurrentTask) {
        TaskControlBlock_t *pxCurrentTCB = (TaskControlBlock_t *)xCurrentTask;
        TaskControlBlock_t *pxNextTCB = (TaskControlBlock_t *)xNextTask;
        
        if (pxNextTCB->ulPriority < pxCurrentTCB->ulPriority) {
            /* Higher priority task is ready, trigger context switch */
            vTriggerContextSwitch();
        }
    }
}
