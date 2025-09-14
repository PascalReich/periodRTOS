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

/* vConfigureSystemClock is now implemented in src/hal/stm32_hal.c */

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
