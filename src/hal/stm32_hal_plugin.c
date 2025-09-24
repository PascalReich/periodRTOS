/**
 * @file stm32_hal_plugin.c
 * @brief STM32 HAL plugin implementation for periodRTOS
 */

#include "hal_interface.h"
#include "periodRTOS.h"
#include "stm32f303xx.h"

/* External variables */
extern uint32_t SystemCoreClock;

/* Systick configuration */
#define SYSTICK_RELOAD_VALUE    (SystemCoreClock / SYSTICK_FREQ_HZ - 1)
#define SYSTICK_PRIORITY        0

/**
 * @brief Initialize STM32 HAL
 */
static void vSTM32HALInit(void)
{
    /* System initialization is handled by vSTM32HALSystemInit */
}

/**
 * @brief Configure system clock
 */
static void vSTM32HALConfigureClock(void)
{
    /* For STM32F3: keep default HSI (8MHz). Optionally enable HSE and set prescalers later. */
    SystemCoreClock = 8000000;
}

/**
 * @brief Initialize GPIO
 */
static void vSTM32HALInitGPIO(void)
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
 * @brief Initialize system timer
 */
static void vSTM32HALInitTimer(void)
{
    /* Configure Systick */
    SysTick->LOAD = SYSTICK_RELOAD_VALUE;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk | 
                   SysTick_CTRL_ENABLE_Msk;
    
    /* Set Systick priority (highest priority) */
    vSTM32HALNVICSetPriority(SysTick_IRQn, SYSTICK_PRIORITY);
}

/**
 * @brief Turn on LED
 */
static void vSTM32HALLedOn(uint32_t ulLed)
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
static void vSTM32HALLedOff(uint32_t ulLed)
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
static void vSTM32HALLedToggle(uint32_t ulLed)
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
 * @brief Set interrupt priority
 */
static void vSTM32HALNVICSetPriority(int irq, uint32_t priority)
{
    if (irq < 0) {
        /* System exceptions */
        SCB->SHPR[((uint32_t)irq & 0xF) - 4] = (priority << (8 - __NVIC_PRIO_BITS)) & 0xFF;
    } else {
        /* External interrupts */
        NVIC->IP[irq] = (priority << (8 - __NVIC_PRIO_BITS)) & 0xFF;
    }
}

/**
 * @brief Enable interrupt
 */
static void vSTM32HALNVICEnableIRQ(int irq)
{
    if (irq >= 0) {
        NVIC->ISER[irq >> 5] = (1UL << (irq & 0x1F));
    }
}

/**
 * @brief Disable interrupt
 */
static void vSTM32HALNVICDisableIRQ(int irq)
{
    if (irq >= 0) {
        NVIC->ICER[irq >> 5] = (1UL << (irq & 0x1F));
    }
}

/**
 * @brief Get system core clock
 */
static uint32_t ulSTM32HALGetSystemClock(void)
{
    return SystemCoreClock;
}

/**
 * @brief System initialization
 */
static void vSTM32HALSystemInit(void)
{
    /* Disable watchdog to prevent resets */
    WWDG->CR &= ~WWDG_CR_WDGA;  /* Disable watchdog */
    
    /* Minimal SystemInit - just set the system clock variable (STM32F3 HSI = 8MHz) */
    SystemCoreClock = 8000000;
}

/**
 * @brief Get HAL name
 */
static const char* pcGetSTM32HALName(void)
{
    return "STM32 HAL";
}

/**
 * @brief Get HAL version
 */
static const char* pcGetSTM32HALVersion(void)
{
    return "1.0.0";
}

/* STM32 HAL interface implementation */
static const hal_interface_t xSTM32HALInterface = {
    .init = vSTM32HALInit,
    .configure_clock = vSTM32HALConfigureClock,
    .init_gpio = vSTM32HALInitGPIO,
    .init_timer = vSTM32HALInitTimer,
    .led_on = vSTM32HALLedOn,
    .led_off = vSTM32HALLedOff,
    .led_toggle = vSTM32HALLedToggle,
    .nvic_set_priority = vSTM32HALNVICSetPriority,
    .nvic_enable_irq = vSTM32HALNVICEnableIRQ,
    .nvic_disable_irq = vSTM32HALNVICDisableIRQ,
    .get_system_clock = ulSTM32HALGetSystemClock,
    .system_init = vSTM32HALSystemInit,
    .get_name = pcGetSTM32HALName,
    .get_version = pcGetSTM32HALVersion
};

/**
 * @brief Get STM32 HAL interface
 */
const hal_interface_t* pxGetSTM32HALInterface(void)
{
    return &xSTM32HALInterface;
}
