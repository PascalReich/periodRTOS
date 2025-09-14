/**
 * @file kernel.c
 * @brief Core kernel implementation for periodRTOS
 */

#include "periodRTOS.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Global variables */
TaskControlBlock_t xTaskList[MAX_TASKS];
static TaskHandle_t xCurrentTask = NULL;
static TaskHandle_t xIdleTask = NULL;
static uint32_t ulNextTaskID = 1;
static uint32_t ulTaskCount = 0;
static SchedulerState_t eSchedulerState = SCHEDULER_NOT_STARTED;
static SystemMonitor_t xSystemMonitor = {0};

/* External function prototypes */
extern void vSchedulerInit(void);
extern TaskHandle_t vSchedulerGetNextTask(void);
extern void vTriggerContextSwitch(void);

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
    
    /* Start the first task */
    vContextSwitch();
}

/**
 * @brief Yield current task
 */
void vTaskYield(void)
{
    if (eSchedulerState == SCHEDULER_RUNNING) {
        vContextSwitch();
    }
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
    pxTCB->pvParameters = pvParameters;
    pxTCB->ulPeriod = ulPeriod;
    pxTCB->ulDeadline = ulDeadline;
    pxTCB->ulStackSize = ulStackSize / sizeof(uint32_t); /* Convert to words */
    pxTCB->ulTaskID = ulNextTaskID++;
    
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

/**
 * @brief Setup task stack
 */
static void vSetupTaskStack(TaskControlBlock_t *pxTCB)
{
    uint32_t *pxStack;
    
    /* Allocate stack memory */
    pxStack = (uint32_t *)malloc(pxTCB->ulStackSize * sizeof(uint32_t));
    if (pxStack == NULL) {
        /* Handle error - for now, we'll use a static allocation approach */
        /* This will be improved in the next iteration */
        return;
    }
    
    pxTCB->pxStack = pxStack;
    pxTCB->pxTopOfStack = &pxStack[pxTCB->ulStackSize - 1];
    
    /* Initialize stack with initial context */
    /* This will be implemented in the context switching module */
}

/**
 * @brief Find free task slot
 */
static TaskHandle_t pxFindFreeTaskSlot(void)
{
    uint32_t i;
    
    for (i = 0; i < MAX_TASKS; i++) {
        if (xTaskList[i].ulTaskID == 0) {
            return (TaskHandle_t)&xTaskList[i];
        }
    }
    
    return NULL;
}
