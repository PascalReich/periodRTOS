/**
 * @file bsp_manager.c
 * @brief BSP manager for periodRTOS
 */

#include "bsp_interface.h"

/* Current BSP interface */
static const bsp_interface_t* pxCurrentBSP = NULL;

/**
 * @brief Get the current BSP interface
 */
const bsp_interface_t* pxGetBSPInterface(void)
{
    return pxCurrentBSP;
}

/**
 * @brief Set the BSP interface
 */
void vSetBSPInterface(const bsp_interface_t* pxInterface)
{
    if (pxInterface != NULL) {
        pxCurrentBSP = pxInterface;
    }
}

/**
 * @brief Initialize BSP (wrapper)
 */
void vBSPInit(void)
{
    if (pxCurrentBSP != NULL && pxCurrentBSP->init != NULL) {
        pxCurrentBSP->init();
    }
}

/**
 * @brief Turn on LED (wrapper)
 */
void vBSPLedOn(uint32_t ulLed)
{
    if (pxCurrentBSP != NULL && pxCurrentBSP->led_on != NULL) {
        pxCurrentBSP->led_on(ulLed);
    }
}

/**
 * @brief Turn off LED (wrapper)
 */
void vBSPLedOff(uint32_t ulLed)
{
    if (pxCurrentBSP != NULL && pxCurrentBSP->led_off != NULL) {
        pxCurrentBSP->led_off(ulLed);
    }
}

/**
 * @brief Toggle LED (wrapper)
 */
void vBSPLedToggle(uint32_t ulLed)
{
    if (pxCurrentBSP != NULL && pxCurrentBSP->led_toggle != NULL) {
        pxCurrentBSP->led_toggle(ulLed);
    }
}

/**
 * @brief Initialize UART (wrapper)
 */
bool bBSPUARTInit(uint32_t baud_rate)
{
    if (pxCurrentBSP != NULL && pxCurrentBSP->uart_init != NULL) {
        return pxCurrentBSP->uart_init(baud_rate);
    }
    return false;
}

/**
 * @brief Send character via UART (wrapper)
 */
void vBSPUARTPutc(char c)
{
    if (pxCurrentBSP != NULL && pxCurrentBSP->uart_putc != NULL) {
        pxCurrentBSP->uart_putc(c);
    }
}

/**
 * @brief Get character from UART (wrapper)
 */
int iBSPUARTGetc(void)
{
    if (pxCurrentBSP != NULL && pxCurrentBSP->uart_getc != NULL) {
        return pxCurrentBSP->uart_getc();
    }
    return -1;
}
