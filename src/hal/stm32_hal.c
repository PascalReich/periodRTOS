/**
 * @file stm32_hal.c
 * @brief Simple STM32 HAL implementation for periodRTOS
 */

#include "stm32f4xx.h"

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
    
    /* Minimal SystemInit - just set the system clock variable */
    SystemCoreClock = 16000000;  /* Use 16MHz HSI for now */
    
    /* TODO: Add proper initialization later when system is stable */
}

/**
 * @brief Get system core clock
 */
uint32_t SystemCoreClock = 168000000;

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
    /* Enable HSE (High Speed External oscillator) */
    RCC->CR |= RCC_CR_HSEON;
    
    /* Wait for HSE to be ready */
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    /* Configure PLL */
    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSE |  /* HSE as PLL source */
                   (8 << RCC_PLLCFGR_PLLM_Pos) |  /* PLLM = 8 */
                   (336 << RCC_PLLCFGR_PLLN_Pos) | /* PLLN = 336 */
                   (0 << RCC_PLLCFGR_PLLP_Pos) |  /* PLLP = 2 */
                   (7 << RCC_PLLCFGR_PLLQ_Pos);   /* PLLQ = 7 */
    
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    
    /* Wait for PLL to be ready */
    while (!(RCC->CR & RCC_CR_PLLRDY));
    
    /* Configure AHB, APB1, APB2 prescalers */
    RCC->CFGR = RCC_CFGR_HPRE_DIV1 |      /* AHB = 168MHz */
                RCC_CFGR_PPRE1_DIV4 |     /* APB1 = 42MHz */
                RCC_CFGR_PPRE2_DIV2;      /* APB2 = 84MHz */
    
    /* Select PLL as system clock */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    
    /* Wait for PLL to be selected */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    /* Update SystemCoreClock variable */
    SystemCoreClock = 168000000;
}
