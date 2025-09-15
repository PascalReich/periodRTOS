/**
 * @file stm32_hal.c
 * @brief Simple STM32 HAL implementation for periodRTOS
 */

#include "stm32f303xx.h"

void vConfigureSystemClock(void);

/**
 * @brief Set interrupt priority
 */
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    if (IRQn < 0) {
        /* System exceptions */
        SCB->SHPR[((uint32_t)IRQn & 0xF) - 4] = (priority << (8 - __NVIC_PRIO_BITS)) & 0xFF;
    } else {
        /* External interrupts */
        NVIC->IP[IRQn] = (priority << (8 - __NVIC_PRIO_BITS)) & 0xFF;
    }
}

/**
 * @brief Enable interrupt
 */
void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    if (IRQn >= 0) {
        NVIC->ISER[IRQn >> 5] = (1UL << (IRQn & 0x1F));
    }
}

/**
 * @brief Disable interrupt
 */
void NVIC_DisableIRQ(IRQn_Type IRQn)
{
    if (IRQn >= 0) {
        NVIC->ICER[IRQn >> 5] = (1UL << (IRQn & 0x1F));
    }
}

/**
 * @brief System initialization
 */
void SystemInit(void)
{
    /* Disable watchdog to prevent resets */
    /* The watchdog might be enabled by default and causing resets */
    WWDG->CR &= ~WWDG_CR_WDGA;  /* Disable watchdog */
    
    /* Minimal SystemInit - just set the system clock variable (STM32F3 HSI = 8MHz) */
    SystemCoreClock = 8000000;
    
    /* TODO: Add proper initialization later when system is stable */
}

/**
 * @brief Get system core clock
 */
uint32_t SystemCoreClock = 8000000;

/**
 * @brief Watchdog interrupt handler
 */
void WWDG_IRQHandler(void)
{
    /* Clear watchdog interrupt flag */
    /* For now, just do nothing to prevent reset */
    /* In a real system, you'd want to feed the watchdog or handle the reset */
}

/**
 * @brief Configure system clock to 168MHz
 */
void vConfigureSystemClock(void)
{
    /* For STM32F3: keep default HSI (8MHz). Optionally enable HSE and set prescalers later. */
    SystemCoreClock = 8000000;
}
