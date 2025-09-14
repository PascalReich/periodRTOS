/**
 * @file monitor.c
 * @brief Monitoring and debugging system for periodRTOS
 */

#include "periodRTOS.h"

/* External variables */
extern TaskControlBlock_t xTaskList[MAX_TASKS];
extern SystemMonitor_t xSystemMonitor;

/**
 * @brief Get total context switch count
 */
uint32_t ulGetContextSwitchCount(void)
{
    return xSystemMonitor.ulTotalContextSwitches;
}

/**
 * @brief Get task execution time
 */
uint32_t ulGetTaskExecutionTime(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (!bIsValidTaskHandle(xTask)) {
        return 0;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    return pxTCB->ulExecutionTime;
}

/**
 * @brief Get task deadline miss count
 */
uint32_t ulGetDeadlineMissCount(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (!bIsValidTaskHandle(xTask)) {
        return 0;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    return pxTCB->ulDeadlineMissCount;
}

/**
 * @brief Check if task deadline is missed
 */
bool bIsTaskDeadlineMissed(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (!bIsValidTaskHandle(xTask)) {
        return false;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    return pxTCB->bDeadlineMissed;
}

/**
 * @brief Get system monitor structure
 */
SystemMonitor_t* pxGetSystemMonitor(void)
{
    return &xSystemMonitor;
}

/**
 * @brief Update context switch count
 */
void vUpdateContextSwitchCount(void)
{
    xSystemMonitor.ulTotalContextSwitches++;
}

/**
 * @brief Update task context switch count
 */
void vUpdateTaskContextSwitchCount(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    
    if (!bIsValidTaskHandle(xTask)) {
        return;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    pxTCB->ulContextSwitchCount++;
}

/**
 * @brief Update idle time
 */
void vUpdateIdleTime(uint32_t ulIdleTicks)
{
    xSystemMonitor.ulIdleTime += ulIdleTicks;
}

/**
 * @brief Get task utilization percentage
 */
uint32_t ulGetTaskUtilization(TaskHandle_t xTask)
{
    TaskControlBlock_t *pxTCB;
    uint32_t ulUtilization;
    
    if (!bIsValidTaskHandle(xTask)) {
        return 0;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    
    if (pxTCB->ulPeriod == 0 || xSystemMonitor.ulSystemUptime == 0) {
        return 0;
    }
    
    /* Calculate utilization as (execution_time / period) * 100 */
    ulUtilization = (pxTCB->ulExecutionTime * 100) / pxTCB->ulPeriod;
    
    return ulUtilization;
}

/**
 * @brief Get system utilization percentage
 */
uint32_t ulGetSystemUtilization(void)
{
    uint32_t ulTotalUtilization = 0;
    TaskControlBlock_t *pxTCB;
    
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        pxTCB = &xTaskList[i];
        if (pxTCB->ulTaskID != 0 && pxTCB->ulPeriod > 0) {
            ulTotalUtilization += ulGetTaskUtilization((TaskHandle_t)pxTCB);
        }
    }
    
    return ulTotalUtilization;
}

/**
 * @brief Get task information
 */
void vGetTaskInfo(TaskHandle_t xTask, char *pcBuffer, uint32_t ulBufferSize)
{
    TaskControlBlock_t *pxTCB;
    
    if (!bIsValidTaskHandle(xTask) || pcBuffer == NULL || ulBufferSize == 0) {
        return;
    }
    
    pxTCB = (TaskControlBlock_t *)xTask;
    
    /* Format task information */
    snprintf(pcBuffer, ulBufferSize,
             "Task: %s\n"
             "ID: %lu\n"
             "State: %d\n"
             "Priority: %lu\n"
             "Period: %lu ms\n"
             "Deadline: %lu ms\n"
             "Execution Time: %lu ms\n"
             "Context Switches: %lu\n"
             "Deadline Misses: %lu\n"
             "Utilization: %lu%%\n",
             pxTCB->pcTaskName,
             pxTCB->ulTaskID,
             pxTCB->eCurrentState,
             pxTCB->ulPriority,
             pxTCB->ulPeriod,
             pxTCB->ulDeadline,
             pxTCB->ulExecutionTime,
             pxTCB->ulContextSwitchCount,
             pxTCB->ulDeadlineMissCount,
             ulGetTaskUtilization(xTask));
}

/**
 * @brief Get system information
 */
void vGetSystemInfo(char *pcBuffer, uint32_t ulBufferSize)
{
    if (pcBuffer == NULL || ulBufferSize == 0) {
        return;
    }
    
    /* Format system information */
    snprintf(pcBuffer, ulBufferSize,
             "System Uptime: %lu ms\n"
             "Total Context Switches: %lu\n"
             "Idle Time: %lu ms\n"
             "Task Count: %lu\n"
             "System Utilization: %lu%%\n"
             "Scheduler State: %d\n",
             xSystemMonitor.ulSystemUptime,
             xSystemMonitor.ulTotalContextSwitches,
             xSystemMonitor.ulIdleTime,
             xSystemMonitor.ulTaskCount,
             ulGetSystemUtilization(),
             xSystemMonitor.eSchedulerState);
}

/**
 * @brief Reset monitoring data
 */
void vResetMonitoringData(void)
{
    TaskControlBlock_t *pxTCB;
    
    /* Reset system monitor */
    xSystemMonitor.ulTotalContextSwitches = 0;
    xSystemMonitor.ulIdleTime = 0;
    
    /* Reset task monitoring data */
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        pxTCB = &xTaskList[i];
        if (pxTCB->ulTaskID != 0) {
            pxTCB->ulExecutionTime = 0;
            pxTCB->ulContextSwitchCount = 0;
            pxTCB->ulDeadlineMissCount = 0;
            pxTCB->bDeadlineMissed = false;
        }
    }
}
