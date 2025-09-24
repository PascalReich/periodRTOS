/**
 * @file kernel.c
 * @brief Core kernel implementation for periodRTOS
 */

#include "periodRTOS.h"
#include <string.h>
#include <stdio.h>

/* Global variables */
TaskControlBlock_t xTaskList[MAX_TASKS];
TaskHandle_t xCurrentTask = NULL;
TaskHandle_t xIdleTask = NULL;
static uint32_t ulNextTaskID = 1;
static uint32_t ulTaskCount = 0;
static SchedulerState_t eSchedulerState = SCHEDULER_NOT_STARTED;
SystemMonitor_t xSystemMonitor = {0};

/* Each task gets a fixed stack size allocated at compile time */
//uint32_t ulStackMemory[MAX_TASKS][DEFAULT_STACK_SIZE / sizeof(uint32_t)];
uint32_t ulStackMemory[MAX_TASKS * DEFAULT_STACK_SIZE / sizeof(uint32_t)];
uint32_t ulStackAllocated[MAX_TASKS] = {0};
uint32_t ulGlobalStackPtr = 0;

#if ENABLE_STACK_CANARY
uint32_t * ulCanaryAddresses[MAX_TASKS];
#endif

/* External function prototypes */
extern void vSchedulerInit(void);
extern TaskHandle_t vSchedulerGetNextTask(void);
extern void vRemoveTaskFromReadyList(TaskHandle_t xTask);
extern void vTriggerContextSwitch(void);

extern uint32_t ulSystemTick;


/* Internal function prototypes */
static void vInitializeTaskControlBlock(TaskControlBlock_t *pxTCB, 
                                       TaskFunction_t pxTaskCode,
                                       const char *pcName,
                                       uint32_t ulStackSize,
                                       void *pvParameters,
                                       uint32_t ulPeriod,
                                       uint32_t ulDeadline);
static void vSetupTaskStack(TaskControlBlock_t *pxTCB);
static TaskHandle_t pxFindFreeTaskSlot(void);

/**
 * @brief Initialize the kernel
 */
void vKernelInit(void)
{
    /* Clear task list */
    memset(xTaskList, 0, sizeof(xTaskList));
    
    /* Initialize system monitor */
    xSystemMonitor.ulTotalContextSwitches = 0;
    xSystemMonitor.ulSystemUptime = 0;
    xSystemMonitor.ulIdleTime = 0;
    xSystemMonitor.ulTaskCount = 0;
    xSystemMonitor.eSchedulerState = SCHEDULER_NOT_STARTED;
    
    /* Reset task counters */
    ulNextTaskID = 1;
    ulTaskCount = 0;
    xCurrentTask = NULL;
    xIdleTask = NULL;
    eSchedulerState = SCHEDULER_NOT_STARTED;
}

/**
 * @brief Create a periodic task
 */
TaskHandle_t xTaskCreatePeriodic(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                uint32_t ulStackSize,
                                void *pvParameters,
                                uint32_t ulPeriod,
                                uint32_t ulDeadline)
{
    TaskControlBlock_t *pxTCB;
    TaskHandle_t xTaskHandle;
    
    /* Parameter validation */
    if (pxTaskCode == NULL || pcName == NULL) {
        return NULL;
    }
    
    /* Check if we can create more tasks */
    if (ulTaskCount >= MAX_TASKS) {
        return NULL;
    }
    
    /* Validate stack size */
    if (ulStackSize < MIN_STACK_SIZE || ulStackSize > MAX_STACK_SIZE) {
        ulStackSize = DEFAULT_STACK_SIZE;
    }
    
    /* Find free task slot */
    xTaskHandle = pxFindFreeTaskSlot();
    if (xTaskHandle == NULL) {
        return NULL;
    }
    
    pxTCB = (TaskControlBlock_t *)xTaskHandle;
    
    /* Initialize task control block */
    vInitializeTaskControlBlock(pxTCB, pxTaskCode, pcName, ulStackSize, 
                               pvParameters, ulPeriod, ulDeadline);
    
    /* Setup task stack */
    vSetupTaskStack(pxTCB);
    
    /* Set initial state */
    pxTCB->eCurrentState = TASK_STATE_READY;
    
    /* Increment task count */
    ulTaskCount++;
    xSystemMonitor.ulTaskCount = ulTaskCount;
    
    return xTaskHandle;
}

/**
 * @brief Start the scheduler
 */
void vTaskStartScheduler(void)
{
    if (eSchedulerState != SCHEDULER_NOT_STARTED) {
        return; /* Scheduler already started */
    }
    
    /* Create idle task if not already created */
    if (xIdleTask == NULL) {
        xIdleTask = xTaskCreatePeriodic(vIdleTask, "Idle", 
                                       DEFAULT_STACK_SIZE, NULL, 
                                       0, 0); /* No period for idle task */
    }
    
    /* Initialize scheduler */
    vSchedulerInit();
    
    /* Set scheduler state */
    eSchedulerState = SCHEDULER_RUNNING;
    xSystemMonitor.eSchedulerState = SCHEDULER_RUNNING;

    /*Get next task*/
    TaskControlBlock_t* next = (TaskControlBlock_t*) vSchedulerGetNextTask();

    vSetCurrentTask(next);

    asm volatile (
        "mov r0, %0"        // Move the value of the operand into r1
        :                   // No output operands
        : "r" (next)        // Input operand: next, assigned to a general-purpose register
        : "r0"              // Clobbered register: r1 is modified by the assembly instruction
    );
    
    /* Start the first task */
    vInitialContextSwitch();
}

/**
 * Assumes curr has updated state. 
 */
void vStartContextSwitch() {

    /*Get task*/
    TaskControlBlock_t* curr = (TaskControlBlock_t*) pxGetCurrentTask();
    TaskControlBlock_t* next = (TaskControlBlock_t*) vSchedulerGetNextTask();

    curr->ulExecutionTime += ulSystemTick - curr->ulLastStartTime;
    //if (curr->eCurrentState == TASK_STATE_RUNNING) curr->eCurrentState = TASK_STATE_READY; // move to ready iff. preempted.

    next->eCurrentState = TASK_STATE_RUNNING;
    next->ulLastStartTime = ulSystemTick;
    next->ulContextSwitchCount++;

    vSetCurrentTask(next);

    xSystemMonitor.ulTotalContextSwitches++;

    //curr->eCurrentState = TASK_STATE_BLOCKED;
    //TODO move to ready? iff preempted?

    asm volatile (
        "mov r0, %0;"        // Move the current task of the operand into r0
        "mov r1, %1;"        // Move the next of the operand into r1
        :                   // No output operands
        : "r" (curr), "r" (next)        // Input operands
        : "r0", "r1"              // Clobbered register: r1 is modified by the assembly instruction
    );

    vContextSwitch();
    
}

/**
 * @brief Yield current task TODO this may be whats leading to state issues
 */
void vTaskYield(void)
{
    if (eSchedulerState == SCHEDULER_RUNNING) {
        TaskControlBlock_t* curr = (TaskControlBlock_t*) pxGetCurrentTask();

        if (curr->ulTaskID || vSchedulerGetNextTask() != curr) {
            curr->eCurrentState = TASK_STATE_BLOCKED;
            vRemoveTaskFromReadyList(curr);
            vStartContextSwitch();
        }
    }
}

void TaskWrapper(void) {
    TaskControlBlock_t* curr = (TaskControlBlock_t*) pxGetCurrentTask();

    curr->taskFlags = 0;

    void (* taskfunc)(void) = (void *) curr->pxTaskCode;


    taskfunc();

    // probably wont return. but if it does, its a instance based task, so we reset the flags to spawn new.

    curr->taskFlags = 1;
    //curr->pxTopOfStack = curr->pxStackBase + (curr->ulStackSize * 4) - 10;
    return vTaskYield();

}


__attribute__((naked)) void task_epilogue() {

}

/**
 * @brief Suspend a task
 */
void vTaskSuspend(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (!bIsValidTaskHandle(xTask)) {
        return;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    pxTCB->eCurrentState = TASK_STATE_SUSPENDED;
}

/**
 * @brief Resume a suspended task
 */
void vTaskResume(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (!bIsValidTaskHandle(xTask)) {
        return;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    if (pxTCB->eCurrentState == TASK_STATE_SUSPENDED) {
        pxTCB->eCurrentState = TASK_STATE_READY;
    }
}

/**
 * @brief Get current task handle
 */
TaskHandle_t pxGetCurrentTask(void)
{
    return xCurrentTask;
}

/**
 * @brief Set current task
 */
void vSetCurrentTask(TaskHandle_t xTask)
{
    xCurrentTask = xTask;
}

/**
 * @brief Get system monitor
 */
SystemMonitor_t* pxGetSystemMonitor(void)
{
    return &xSystemMonitor;
}

/**
 * @brief Validate task handle
 */
bool bIsValidTaskHandle(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (xTask == NULL) {
        return false;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    
    /* Check if task is within valid range */
    if (pxTCB < &xTaskList[0] || pxTCB >= &xTaskList[MAX_TASKS]) {
        return false;
    }
    
    /* Check if task is initialized */
    if (pxTCB->ulTaskID == 0) {
        return false;
    }
    
    return true;
}

/**
 * @brief Initialize task control block
 */
static void vInitializeTaskControlBlock(TaskControlBlock_t *pxTCB, 
                                       TaskFunction_t pxTaskCode,
                                       const char *pcName,
                                       uint32_t ulStackSize,
                                       void *pvParameters,
                                       uint32_t ulPeriod,
                                       uint32_t ulDeadline)
{
    /* Clear the TCB */
    memset(pxTCB, 0, sizeof(TaskControlBlock_t));
    
    /* Set task properties */
    pxTCB->pxTaskCode = pxTaskCode;
    pxTCB->taskFlags = 1;
    pxTCB->pvParameters = pvParameters;
    pxTCB->ulPeriod = ulPeriod;
    pxTCB->ulDeadline = ulDeadline;
    pxTCB->ulStackSize = ulStackSize / sizeof(uint32_t); /* Convert to words */
    if (!ulPeriod){ // this should be the idle task
        if (!xIdleTask) { // idle task not init yet
            pxTCB->ulTaskID = 0;
        } else { // second idle task??
            *((void*)0x0); // deref a null ptr
        }
    } else {
        pxTCB->ulTaskID = ulNextTaskID++;
    }
    
    /* Copy task name */
    strncpy(pxTCB->pcTaskName, pcName, sizeof(pxTCB->pcTaskName) - 1);
    pxTCB->pcTaskName[sizeof(pxTCB->pcTaskName) - 1] = '\0';
    
    /* Initialize timing */
    pxTCB->ulReleaseTime = 0;
    pxTCB->ulDeadlineTime = 0;
    pxTCB->ulExecutionTime = 0;
    pxTCB->ulLastStartTime = 0;
    
    /* Initialize monitoring */
    pxTCB->ulContextSwitchCount = 0;
    pxTCB->ulDeadlineMissCount = 0;
    pxTCB->bDeadlineMissed = false;
}

static void vInitializeTaskStack(void) {
    TaskControlBlock_t* curr = (TaskControlBlock_t*) pxGetCurrentTask();
    curr->pxTopOfStack = curr->pxStackBase + (curr->ulStackSize * 4) - 10;
}

/**
 * @brief Setup task stack
 */
static void vSetupTaskStack(TaskControlBlock_t *pxTCB)
{
    
    /* Find an available stack slot */
    /*
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        if (ulStackAllocated[i] == 0) {
            ulStackAllocated[i] = 1;
            pxTCB->pxStackBase = ulStackMemory[i]; // bottom limit
            pxTCB->pxTopOfStack = ulStackMemory + pxTCB->ulStackSize; //&ulStackMemory[i][pxTCB->ulStackSize - 1];
            // current pointer
            pxTCB->pxTopOfStack -= 28; // compensate for 8 registers and return pointer
            *pxTCB->pxTopOfStack  = (unsigned int)pxTCB->pxTaskCode;
            return;
        }
    }*/

    #if ENABLE_STACK_CANARY
    ulStackMemory[ulGlobalStackPtr] = STACK_CANARY;
    ulCanaryAddresses[pxTCB->ulTaskID] = &ulStackMemory[ulGlobalStackPtr];
    ulGlobalStackPtr++;
    #endif

    pxTCB->pxStackBase = &ulStackMemory[ulGlobalStackPtr];
    unsigned int bytes = pxTCB->ulStackSize * sizeof(uint32_t);
    
    if (ulGlobalStackPtr + bytes < MAX_TASKS * DEFAULT_STACK_SIZE) {
        pxTCB->pxTopOfStack = pxTCB->pxStackBase + bytes - 1;

        pxTCB->pxTopOfStack -= 9; // compensate for 8 registers and return pointer
        pxTCB->pxStackMax = pxTCB->pxTopOfStack;
        *pxTCB->pxTopOfStack  = (unsigned int)pxTCB->pxTaskCode;

        ulGlobalStackPtr += bytes;

        
        return;
    }

    
    /* If we get here, no stack available */
    pxTCB->pxStackBase = NULL;
    pxTCB->pxTopOfStack = NULL;
}

/**
 * @brief Find free task slot
 */
static TaskHandle_t pxFindFreeTaskSlot(void)
{
    uint32_t i;
    
    for (i = 0; i < MAX_TASKS; i++) {
        
        //if (xTaskList[i].ulTaskID == 0) {
        //    return (TaskHandle_t)&xTaskList[i];
        //}
        

        return &xTaskList[ulNextTaskID];

    }
    
    return NULL;
}
