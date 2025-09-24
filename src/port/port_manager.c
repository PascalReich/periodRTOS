/**
 * @file port_manager.c
 * @brief Port layer manager for periodRTOS
 */

#include "port_interface.h"

/* Current port interface */
static const port_interface_t* pxCurrentPort = NULL;

/**
 * @brief Get the current port interface
 */
const port_interface_t* pxGetPortInterface(void)
{
    return pxCurrentPort;
}

/**
 * @brief Set the port interface
 */
void vSetPortInterface(const port_interface_t* pxInterface)
{
    if (pxInterface != NULL) {
        pxCurrentPort = pxInterface;
    }
}

/**
 * @brief Initialize port layer (wrapper)
 */
void vPortInit(void)
{
    if (pxCurrentPort != NULL && pxCurrentPort->init != NULL) {
        pxCurrentPort->init();
    }
}

/**
 * @brief Initialize timer (wrapper)
 */
bool bPortTimerInit(uint32_t tick_freq_hz)
{
    if (pxCurrentPort != NULL && pxCurrentPort->timer_init != NULL) {
        return pxCurrentPort->timer_init(tick_freq_hz);
    }
    return false;
}

/**
 * @brief Get tick count (wrapper)
 */
uint32_t ulPortGetTickCount(void)
{
    if (pxCurrentPort != NULL && pxCurrentPort->get_tick_count != NULL) {
        return pxCurrentPort->get_tick_count();
    }
    return 0;
}

/**
 * @brief Context switch (wrapper)
 */
void vPortContextSwitch(void)
{
    if (pxCurrentPort != NULL && pxCurrentPort->context_switch != NULL) {
        pxCurrentPort->context_switch();
    }
}

/**
 * @brief Task yield (wrapper)
 */
void vPortYield(void)
{
    if (pxCurrentPort != NULL && pxCurrentPort->yield != NULL) {
        pxCurrentPort->yield();
    }
}

/**
 * @brief Disable interrupts (wrapper)
 */
uint32_t ulPortDisableInterrupts(void)
{
    if (pxCurrentPort != NULL && pxCurrentPort->disable_interrupts != NULL) {
        return pxCurrentPort->disable_interrupts();
    }
    return 0;
}

/**
 * @brief Enable interrupts (wrapper)
 */
void vPortEnableInterrupts(uint32_t state)
{
    if (pxCurrentPort != NULL && pxCurrentPort->enable_interrupts != NULL) {
        pxCurrentPort->enable_interrupts(state);
    }
}

/**
 * @brief Get system clock (wrapper)
 */
uint32_t ulPortGetSystemClock(void)
{
    if (pxCurrentPort != NULL && pxCurrentPort->get_system_clock != NULL) {
        return pxCurrentPort->get_system_clock();
    }
    return 0;
}
