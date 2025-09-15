/**
 * @file board_init.c
 * @brief STM32F3 (e.g., STM32F303 Discovery) board initialization
 */

#include "periodRTOS.h"
#include "stm32f303xx.h"

/* External variables */
extern uint32_t SystemCoreClock;

/**
 * @brief Initialize STM32F3 Discovery board
 */
void vBoardInit(void)
{
    /* Enable system clock */
    SystemInit();
    
    /* Keep default HSI at 8MHz for F3 for now */
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
    /* Enable GPIO clocks (F3 uses AHBENR) */
    RCC->AHBENR |= RCC_AHBENR_IOPEEN;  /* Enable GPIOE clock */
    
    /* Configure LED pins for STM32F3DISCOVERY: PE8..PE15 */
    GPIOE->MODER |= (GPIO_MODER_MODER8_0  |
                     GPIO_MODER_MODER9_0  |
                     GPIO_MODER_MODER10_0 |
                     GPIO_MODER_MODER11_0 |
                     GPIO_MODER_MODER12_0 |
                     GPIO_MODER_MODER13_0 |
                     GPIO_MODER_MODER14_0 |
                     GPIO_MODER_MODER15_0);
    
    /* Set initial LED states (all off) */
    GPIOE->ODR &= ~(GPIO_ODR_OD8 | GPIO_ODR_OD9 | GPIO_ODR_OD10 | GPIO_ODR_OD11 |
                    GPIO_ODR_OD12 | GPIO_ODR_OD13 | GPIO_ODR_OD14 | GPIO_ODR_OD15);
}

/**
 * @brief Turn on LED
 */
void vLedOn(uint32_t ulLed)
{
    switch (ulLed) {
        case 0: GPIOE->ODR |= GPIO_ODR_OD8; break;
        case 1: GPIOE->ODR |= GPIO_ODR_OD9; break;
        case 2: GPIOE->ODR |= GPIO_ODR_OD10; break;
        case 3: GPIOE->ODR |= GPIO_ODR_OD11; break;
        case 4: GPIOE->ODR |= GPIO_ODR_OD12; break;
        case 5: GPIOE->ODR |= GPIO_ODR_OD13; break;
        case 6: GPIOE->ODR |= GPIO_ODR_OD14; break;
        case 7: GPIOE->ODR |= GPIO_ODR_OD15; break;
    }
}

/**
 * @brief Turn off LED
 */
void vLedOff(uint32_t ulLed)
{
    switch (ulLed) {
        case 0: GPIOE->ODR &= ~GPIO_ODR_OD8; break;
        case 1: GPIOE->ODR &= ~GPIO_ODR_OD9; break;
        case 2: GPIOE->ODR &= ~GPIO_ODR_OD10; break;
        case 3: GPIOE->ODR &= ~GPIO_ODR_OD11; break;
        case 4: GPIOE->ODR &= ~GPIO_ODR_OD12; break;
        case 5: GPIOE->ODR &= ~GPIO_ODR_OD13; break;
        case 6: GPIOE->ODR &= ~GPIO_ODR_OD14; break;
        case 7: GPIOE->ODR &= ~GPIO_ODR_OD15; break;
    }
}

/**
 * @brief Toggle LED
 */
void vLedToggle(uint32_t ulLed)
{
    switch (ulLed) {
        case 0: GPIOE->ODR ^= GPIO_ODR_OD8; break;
        case 1: GPIOE->ODR ^= GPIO_ODR_OD9; break;
        case 2: GPIOE->ODR ^= GPIO_ODR_OD10; break;
        case 3: GPIOE->ODR ^= GPIO_ODR_OD11; break;
        case 4: GPIOE->ODR ^= GPIO_ODR_OD12; break;
        case 5: GPIOE->ODR ^= GPIO_ODR_OD13; break;
        case 6: GPIOE->ODR ^= GPIO_ODR_OD14; break;
        case 7: GPIOE->ODR ^= GPIO_ODR_OD15; break;
    }
}
