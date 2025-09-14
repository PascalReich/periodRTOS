/**
 * @file board_init.c
 * @brief STM32F4 Discovery board initialization
 */

#include "periodRTOS.h"
#include "stm32f4xx.h"

/* External variables */
extern uint32_t SystemCoreClock;

/**
 * @brief Initialize STM32F4 Discovery board
 */
void vBoardInit(void)
{
    /* Enable system clock */
    SystemInit();
    
    /* Configure system clock to 168MHz */
    vConfigureSystemClock();
    
    /* Initialize GPIO for LEDs */
    vInitGPIO();
    
    /* Initialize Systick */
    vSystickInit();
    
    /* Initialize kernel */
    vKernelInit();
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
    
    /* Configure flash latency */
    FLASH->ACR = FLASH_ACR_LATENCY_5WS |  /* 5 wait states for 168MHz */
                 FLASH_ACR_PRFTEN |        /* Prefetch enable */
                 FLASH_ACR_ICEN |          /* Instruction cache enable */
                 FLASH_ACR_DCEN;           /* Data cache enable */
    
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

/**
 * @brief Initialize GPIO for LEDs
 */
void vInitGPIO(void)
{
    /* Enable GPIO clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;  /* Enable GPIOG clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  /* Enable GPIOD clock */
    
    /* Configure LED pins (PG13, PG14, PG15, PD12, PD13, PD14, PD15) */
    GPIOG->MODER |= GPIO_MODER_MODER13_0 |  /* PG13 output */
                    GPIO_MODER_MODER14_0 |  /* PG14 output */
                    GPIO_MODER_MODER15_0;   /* PG15 output */
    
    GPIOD->MODER |= GPIO_MODER_MODER12_0 |  /* PD12 output */
                    GPIO_MODER_MODER13_0 |  /* PD13 output */
                    GPIO_MODER_MODER14_0 |  /* PD14 output */
                    GPIO_MODER_MODER15_0;   /* PD15 output */
    
    /* Set initial LED states (all off) */
    GPIOG->ODR &= ~(GPIO_ODR_OD13 | GPIO_ODR_OD14 | GPIO_ODR_OD15);
    GPIOD->ODR &= ~(GPIO_ODR_OD12 | GPIO_ODR_OD13 | GPIO_ODR_OD14 | GPIO_ODR_OD15);
}

/**
 * @brief Turn on LED
 */
void vLedOn(uint32_t ulLed)
{
    switch (ulLed) {
        case 0: GPIOG->ODR |= GPIO_ODR_OD13; break;
        case 1: GPIOG->ODR |= GPIO_ODR_OD14; break;
        case 2: GPIOG->ODR |= GPIO_ODR_OD15; break;
        case 3: GPIOD->ODR |= GPIO_ODR_OD12; break;
        case 4: GPIOD->ODR |= GPIO_ODR_OD13; break;
        case 5: GPIOD->ODR |= GPIO_ODR_OD14; break;
        case 6: GPIOD->ODR |= GPIO_ODR_OD15; break;
    }
}

/**
 * @brief Turn off LED
 */
void vLedOff(uint32_t ulLed)
{
    switch (ulLed) {
        case 0: GPIOG->ODR &= ~GPIO_ODR_OD13; break;
        case 1: GPIOG->ODR &= ~GPIO_ODR_OD14; break;
        case 2: GPIOG->ODR &= ~GPIO_ODR_OD15; break;
        case 3: GPIOD->ODR &= ~GPIO_ODR_OD12; break;
        case 4: GPIOD->ODR &= ~GPIO_ODR_OD13; break;
        case 5: GPIOD->ODR &= ~GPIO_ODR_OD14; break;
        case 6: GPIOD->ODR &= ~GPIO_ODR_OD15; break;
    }
}

/**
 * @brief Toggle LED
 */
void vLedToggle(uint32_t ulLed)
{
    switch (ulLed) {
        case 0: GPIOG->ODR ^= GPIO_ODR_OD13; break;
        case 1: GPIOG->ODR ^= GPIO_ODR_OD14; break;
        case 2: GPIOG->ODR ^= GPIO_ODR_OD15; break;
        case 3: GPIOD->ODR ^= GPIO_ODR_OD12; break;
        case 4: GPIOD->ODR ^= GPIO_ODR_OD13; break;
        case 5: GPIOD->ODR ^= GPIO_ODR_OD14; break;
        case 6: GPIOD->ODR ^= GPIO_ODR_OD15; break;
    }
}
