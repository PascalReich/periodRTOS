/**
 * @file systick.c
 * @brief Systick timer implementation for periodRTOS
 */

#include "periodRTOS.h"
#include "scheduler_interface.h"

/* Systick configuration */
#define SYSTICK_RELOAD_VALUE    (SystemCoreClock / SYSTICK_FREQ_HZ - 1)
#define SYSTICK_PRIORITY        0

/* External variables */
extern uint32_t SystemCoreClock;
extern uint32_t ulSystemTick;

/**
 * @brief Initialize Systick timer
 */
void vSystickInit(void)
{
    /* Configure Systick */
    SysTick->LOAD = SYSTICK_RELOAD_VALUE;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk | 
                   SysTick_CTRL_ENABLE_Msk;
    
    /* Set Systick priority (highest priority) */
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIORITY);
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
    return ulSystemTick;
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
