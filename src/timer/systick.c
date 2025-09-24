/**
 * @file systick.c
 * @brief Systick timer implementation for periodRTOS
 */

#include "periodRTOS.h"
#include "scheduler_interface.h"
#include "port_interface.h"

/* External variables */
extern uint32_t ulSystemTick;

/**
 * @brief Initialize Systick timer
 */
void vSystickInit(void)
{
    /* Use port layer to initialize timer */
    bPortTimerInit(SYSTICK_FREQ_HZ);
}

/**
 * @brief Systick interrupt handler
 */
void SysTick_Handler(void)
{
    /* Call scheduler tick handler */
    vSchedulerTickHandler();
}

/**
 * @brief Get current system tick
 */
uint32_t ulGetSystemTick(void)
{
    return ulPortGetTickCount();
}

/**
 * @brief Delay for specified number of ticks
 */
void vTaskDelay(uint32_t ulTicksToDelay)
{
    uint32_t ulStartTick = ulSystemTick;
    
    while ((ulSystemTick - ulStartTick) < ulTicksToDelay) {
        /* Yield to other tasks */
        vTaskYield();
    }
}

/**
 * @brief Delay until specified tick
 */
void vTaskDelayUntil(uint32_t *pulPreviousWakeTime, uint32_t ulTimeIncrement)
{
    uint32_t ulNextWakeTime = *pulPreviousWakeTime + ulTimeIncrement;
    uint32_t ulCurrentTick = ulSystemTick;
    
    /* Check if we need to wait */
    if (ulCurrentTick < ulNextWakeTime) {
        vTaskDelay(ulNextWakeTime - ulCurrentTick);
    }
    
    /* Update previous wake time */
    *pulPreviousWakeTime = ulNextWakeTime;
}
