/**
 * @file hal_manager.c
 * @brief HAL plugin manager for periodRTOS
 */

#include "hal_interface.h"

/* Current HAL interface */
static const hal_interface_t* pxCurrentHAL = NULL;

/**
 * @brief Get the current HAL interface
 */
const hal_interface_t* pxGetHALInterface(void)
{
    return pxCurrentHAL;
}

/**
 * @brief Set the HAL interface
 */
void vSetHALInterface(const hal_interface_t* pxInterface)
{
    if (pxInterface != NULL) {
        pxCurrentHAL = pxInterface;
    }
}

/**
 * @brief Initialize HAL (wrapper)
 */
void vHALInit(void)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->init != NULL) {
        pxCurrentHAL->init();
    }
}

/**
 * @brief Configure system clock (wrapper)
 */
void vHALConfigureClock(void)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->configure_clock != NULL) {
        pxCurrentHAL->configure_clock();
    }
}

/**
 * @brief Initialize GPIO (wrapper)
 */
void vHALInitGPIO(void)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->init_gpio != NULL) {
        pxCurrentHAL->init_gpio();
    }
}

/**
 * @brief Initialize timer (wrapper)
 */
void vHALInitTimer(void)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->init_timer != NULL) {
        pxCurrentHAL->init_timer();
    }
}

/**
 * @brief Turn on LED (wrapper)
 */
void vHALLedOn(uint32_t ulLed)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->led_on != NULL) {
        pxCurrentHAL->led_on(ulLed);
    }
}

/**
 * @brief Turn off LED (wrapper)
 */
void vHALLedOff(uint32_t ulLed)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->led_off != NULL) {
        pxCurrentHAL->led_off(ulLed);
    }
}

/**
 * @brief Toggle LED (wrapper)
 */
void vHALLedToggle(uint32_t ulLed)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->led_toggle != NULL) {
        pxCurrentHAL->led_toggle(ulLed);
    }
}

/**
 * @brief Set interrupt priority (wrapper)
 */
void vHALNVICSetPriority(int irq, uint32_t priority)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->nvic_set_priority != NULL) {
        pxCurrentHAL->nvic_set_priority(irq, priority);
    }
}

/**
 * @brief Enable interrupt (wrapper)
 */
void vHALNVICEnableIRQ(int irq)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->nvic_enable_irq != NULL) {
        pxCurrentHAL->nvic_enable_irq(irq);
    }
}

/**
 * @brief Disable interrupt (wrapper)
 */
void vHALNVICDisableIRQ(int irq)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->nvic_disable_irq != NULL) {
        pxCurrentHAL->nvic_disable_irq(irq);
    }
}

/**
 * @brief Get system clock (wrapper)
 */
uint32_t ulHALGetSystemClock(void)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->get_system_clock != NULL) {
        return pxCurrentHAL->get_system_clock();
    }
    return 0;
}

/**
 * @brief System initialization (wrapper)
 */
void vHALSystemInit(void)
{
    if (pxCurrentHAL != NULL && pxCurrentHAL->system_init != NULL) {
        pxCurrentHAL->system_init();
    }
}
