/**
 * @file periodRTOS.h
 * @brief Main header file for periodRTOS - A periodic task focused RTOS
 * @author periodRTOS Team
 * @version 1.0.0
 */

#ifndef PERIODRTOS_H
#define PERIODRTOS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Configuration constants */
#define MAX_TASKS                12
#define MAX_PRIORITY_LEVELS      12
#define DEFAULT_STACK_SIZE       512
#define MAX_STACK_SIZE           2048
#define MIN_STACK_SIZE           128

/* System tick configuration */
#define SYSTICK_FREQ_HZ          1000    /* 1ms tick */
#define SYSTICK_PRIORITY         0       /* Highest priority */

/* Stack Canary*/
#define ENABLE_STACK_CANARY      true
#define STACK_CANARY             0x00ff0a00

/* Task states */
typedef enum {
    TASK_STATE_READY = 0,
    TASK_STATE_RUNNING,
    TASK_STATE_BLOCKED,
    TASK_STATE_SUSPENDED,
    TASK_STATE_DELETED
} TaskState_t;

/* Task handle - opaque pointer */
typedef void* TaskHandle_t;

/* Task function prototype */
typedef void (*TaskFunction_t)(void *parameters);

/* Task control block structure */
typedef struct TaskControlBlock {
    /* Stack management */
    uint32_t *pxTopOfStack;          /* Current stack pointer */
    uint32_t *pxStackBase;               /* Base of stack */
    uint32_t *pxStackMax;               /* Max Addr (starting point) of stack */
    uint32_t ulStackSize;            /* Stack size in words */
    
    /* Task properties */
    TaskFunction_t pxTaskCode;       /* Task function pointer */
    uint32_t taskFlags;              /* 1 is fresh, 0 is dirty*/
    void *pvParameters;              /* Task parameters */
    uint32_t ulPeriod;               /* Task period in ms */
    uint32_t ulDeadline;             /* Task deadline in ms */
    uint32_t ulPriority;             /* Task priority (0 = highest) */
    TaskState_t eCurrentState;       /* Current task state */
    
    /* Timing information */
    uint32_t ulReleaseTime;          /* Next release time */
    uint32_t ulDeadlineTime;         /* Next deadline time */
    uint32_t ulExecutionTime;        /* Total execution time */
    uint32_t ulLastStartTime;        /* Last start execution time */
    
    /* Monitoring data */
    uint32_t ulContextSwitchCount;   /* Number of context switches */
    uint32_t ulDeadlineMissCount;    /* Number of deadline misses */
    bool bDeadlineMissed;            /* Current deadline miss flag */
    
    /* Task identification */
    char pcTaskName[16];             /* Task name for debugging */
    uint32_t ulTaskID;               /* Unique task ID */
} TaskControlBlock_t;

/* Scheduler state */
typedef enum {
    SCHEDULER_NOT_STARTED = 0,
    SCHEDULER_RUNNING,
    SCHEDULER_SUSPENDED
} SchedulerState_t;

/* System monitoring structure */
typedef struct {
    uint32_t ulTotalContextSwitches;
    uint32_t ulSystemUptime;         /* System uptime in ms */
    uint32_t ulIdleTime;             /* Total idle time in ms */
    uint32_t ulTaskCount;            /* Number of created tasks */
    SchedulerState_t eSchedulerState;
} SystemMonitor_t;

/* Function prototypes */

/* Task management */
TaskHandle_t xTaskCreatePeriodic(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                uint32_t ulStackSize,
                                void *pvParameters,
                                uint32_t ulPeriod,
                                uint32_t ulDeadline);

void vTaskStartScheduler(void);
void vTaskYield(void);
void vTaskSuspend(TaskHandle_t xTask);
void vTaskResume(TaskHandle_t xTask);
void vRemoveTaskFromReadyList(TaskHandle_t xTask);

/* Monitoring functions */
uint32_t ulGetContextSwitchCount(void);
uint32_t ulGetTaskExecutionTime(TaskHandle_t xTask);
uint32_t ulGetDeadlineMissCount(TaskHandle_t xTask);
bool bIsTaskDeadlineMissed(TaskHandle_t xTask);
SystemMonitor_t* pxGetSystemMonitor(void);

/* System functions */
void vSystemTickHandler(void);
void vIdleTask(void *pvParameters);

/* Internal kernel functions (not part of public API) */
void vKernelInit(void);
void vSchedulerInit(void);
void vContextSwitch(void);
void vStartContextSwitch(void);
void vInitialContextSwitch(void);
TaskHandle_t pxGetCurrentTask(void);
void vSetCurrentTask(TaskHandle_t xTask);
TaskHandle_t vSchedulerGetNextTask(void);
void vTriggerContextSwitch(void);
bool bIsValidTaskHandle(TaskHandle_t xTask);

/* Scheduler interface functions */
void vSchedulerAddTask(TaskHandle_t xTask);
void vSchedulerRemoveTask(TaskHandle_t xTask);
void vSchedulerUpdatePriorities(void);
void vSchedulerTickHandler(void);

/* Board-specific functions */
void vBoardInit(void);

/* Port layer functions (RTOS-essential only) */
void vPortInit(void);
bool bPortTimerInit(uint32_t tick_freq_hz);
uint32_t ulPortGetTickCount(void);
void vPortContextSwitch(void);
void vPortYield(void);
uint32_t ulPortDisableInterrupts(void);
void vPortEnableInterrupts(uint32_t state);
uint32_t ulPortGetSystemClock(void);

/* BSP functions (optional convenience functions) */
void vBSPInit(void);
void vBSPLedOn(uint32_t ulLed);
void vBSPLedOff(uint32_t ulLed);
void vBSPLedToggle(uint32_t ulLed);
bool bBSPUARTInit(uint32_t baud_rate);
void vBSPUARTPutc(char c);
int iBSPUARTGetc(void);

/* Timer functions */
void vSystickInit(void);
uint32_t ulGetSystemTick(void);
void vTaskDelay(uint32_t ulTicksToDelay);
void vTaskDelayUntil(uint32_t *pulPreviousWakeTime, uint32_t ulTimeIncrement);

#ifdef __cplusplus
}
#endif

#endif /* PERIODRTOS_H */
