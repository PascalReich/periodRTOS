/**
 * @file stm32f3_discovery_bsp.c
 * @brief STM32F3 Discovery BSP implementation for periodRTOS
 * 
 * This file provides optional common peripheral functions for
 * the STM32F3 Discovery board. Applications can use this for
 * convenience or use STM32 HAL directly.
 */

#include "bsp_interface.h"
#include "periodRTOS.h"
#include "stm32f303xx.h"

/**
 * @brief Initialize STM32F3 Discovery BSP
 */
static void vSTM32F3DiscoveryBSPInit(void)
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
static void vSTM32F3DiscoveryBSPLedOn(uint32_t ulLed)
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
static void vSTM32F3DiscoveryBSPLedOff(uint32_t ulLed)
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
static void vSTM32F3DiscoveryBSPLedToggle(uint32_t ulLed)
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

/**
 * @brief Initialize UART (not implemented - use STM32 HAL)
 */
static bool bSTM32F3DiscoveryBSPUARTInit(uint32_t baud_rate)
{
    /* UART initialization should be done using STM32 HAL */
    /* This is just a placeholder */
    (void)baud_rate;  /* Suppress unused parameter warning */
    return false;
}

/**
 * @brief Send character via UART (not implemented - use STM32 HAL)
 */
static void vSTM32F3DiscoveryBSPUARTPutc(char c)
{
    /* UART operations should be done using STM32 HAL */
    (void)c;  /* Suppress unused parameter warning */
}

/**
 * @brief Get character from UART (not implemented - use STM32 HAL)
 */
static int iSTM32F3DiscoveryBSPUARTGetc(void)
{
    /* UART operations should be done using STM32 HAL */
    return -1;
}

/**
 * @brief Get BSP name
 */
static const char* pcGetSTM32F3DiscoveryBSPName(void)
{
    return "STM32F3 Discovery";
}

/**
 * @brief Get BSP version
 */
static const char* pcGetSTM32F3DiscoveryBSPVersion(void)
{
    return "1.0.0";
}

/* STM32F3 Discovery BSP interface implementation */
static const bsp_interface_t xSTM32F3DiscoveryBSPInterface = {
    .init = vSTM32F3DiscoveryBSPInit,
    .led_on = vSTM32F3DiscoveryBSPLedOn,
    .led_off = vSTM32F3DiscoveryBSPLedOff,
    .led_toggle = vSTM32F3DiscoveryBSPLedToggle,
    .uart_init = bSTM32F3DiscoveryBSPUARTInit,
    .uart_putc = vSTM32F3DiscoveryBSPUARTPutc,
    .uart_getc = iSTM32F3DiscoveryBSPUARTGetc,
    .get_name = pcGetSTM32F3DiscoveryBSPName,
    .get_version = pcGetSTM32F3DiscoveryBSPVersion
};

/**
 * @brief Get STM32F3 Discovery BSP interface
 */
const bsp_interface_t* pxGetSTM32F3DiscoveryBSPInterface(void)
{
    return &xSTM32F3DiscoveryBSPInterface;
}
