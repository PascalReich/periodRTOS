/**
 * @file stm32f3_port.c
 * @brief STM32F3 port layer implementation for periodRTOS
 * 
 * This file provides only the minimal hardware functions needed
 * for RTOS operation. Applications should use STM32 HAL for
 * peripheral access.
 */

#include "port_interface.h"
#include "periodRTOS.h"
#include "stm32f303xx.h"

/* External variables */
extern uint32_t SystemCoreClock;
extern uint32_t ulSystemTick;

/* Systick configuration */
#define SYSTICK_PRIORITY        0

/**
 * @brief Initialize STM32F3 port layer
 */
static void vSTM32F3PortInit(void)
{
    /* Disable watchdog to prevent resets */
    WWDG->CR &= ~WWDG_CR_WDGA;
    
    /* Set system clock variable (STM32F3 HSI = 8MHz) */
    SystemCoreClock = 8000000;
}

/**
 * @brief Initialize system timer for RTOS tick
 */
static bool bSTM32F3PortTimerInit(uint32_t tick_freq_hz)
{
    uint32_t reload_value = SystemCoreClock / tick_freq_hz - 1;
    
    /* Configure Systick */
    SysTick->LOAD = reload_value;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk | 
                   SysTick_CTRL_ENABLE_Msk;
    
    /* Set Systick priority (highest priority) */
    if (SysTick_IRQn < 0) {
        /* System exceptions */
        SCB->SHPR[((uint32_t)SysTick_IRQn & 0xF) - 4] = (SYSTICK_PRIORITY << (8 - __NVIC_PRIO_BITS)) & 0xFF;
    } else {
        /* External interrupts */
        NVIC->IP[SysTick_IRQn] = (SYSTICK_PRIORITY << (8 - __NVIC_PRIO_BITS)) & 0xFF;
    }
    
    return true;
}

/**
 * @brief Get current tick count
 */
static uint32_t ulSTM32F3PortGetTickCount(void)
{
    return ulSystemTick;
}

/**
 * @brief Perform context switch
 */
static void vSTM32F3PortContextSwitch(void)
{
    /* Context switching is handled by assembly code */
    /* This function is called by the scheduler */
    extern void vContextSwitch(void);
    vContextSwitch();
}

/**
 * @brief Task yield
 */
static void vSTM32F3PortYield(void)
{
    /* Trigger context switch */
    extern void vTriggerContextSwitch(void);
    vTriggerContextSwitch();
}

/**
 * @brief Disable interrupts
 */
static uint32_t ulSTM32F3PortDisableInterrupts(void)
{
    uint32_t state = __get_PRIMASK();
    __disable_irq();
    return state;
}

/**
 * @brief Enable interrupts
 */
static void vSTM32F3PortEnableInterrupts(uint32_t state)
{
    __set_PRIMASK(state);
}

/**
 * @brief Get system core clock
 */
static uint32_t ulSTM32F3PortGetSystemClock(void)
{
    return SystemCoreClock;
}

/**
 * @brief Get port name
 */
static const char* pcGetSTM32F3PortName(void)
{
    return "STM32F3";
}

/**
 * @brief Get port version
 */
static const char* pcGetSTM32F3PortVersion(void)
{
    return "1.0.0";
}

/* STM32F3 port interface implementation */
static const port_interface_t xSTM32F3PortInterface = {
    .init = vSTM32F3PortInit,
    .timer_init = bSTM32F3PortTimerInit,
    .get_tick_count = ulSTM32F3PortGetTickCount,
    .context_switch = vSTM32F3PortContextSwitch,
    .yield = vSTM32F3PortYield,
    .disable_interrupts = ulSTM32F3PortDisableInterrupts,
    .enable_interrupts = vSTM32F3PortEnableInterrupts,
    .get_system_clock = ulSTM32F3PortGetSystemClock,
    .get_name = pcGetSTM32F3PortName,
    .get_version = pcGetSTM32F3PortVersion
};

/**
 * @brief Get STM32F3 port interface
 */
const port_interface_t* pxGetSTM32F3PortInterface(void)
{
    return &xSTM32F3PortInterface;
}
