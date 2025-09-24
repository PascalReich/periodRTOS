/**
 * @file bsp_interface.h
 * @brief Board Support Package interface for periodRTOS
 * 
 * This interface provides optional common peripheral functions
 * for convenience. Applications can use this or vendor HALs directly.
 */

#ifndef BSP_INTERFACE_H
#define BSP_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NULL 0

/**
 * @brief BSP interface structure
 * 
 * This interface provides optional common peripheral functions.
 * Applications can use this for convenience or use vendor HALs directly.
 */
typedef struct {
    /**
     * @brief Initialize BSP
     */
    void (*init)(void);
    
    /**
     * @brief Turn on LED
     * @param ulLed LED number
     */
    void (*led_on)(uint32_t ulLed);
    
    /**
     * @brief Turn off LED
     * @param ulLed LED number
     */
    void (*led_off)(uint32_t ulLed);
    
    /**
     * @brief Toggle LED
     * @param ulLed LED number
     */
    void (*led_toggle)(uint32_t ulLed);
    
    /**
     * @brief Initialize UART
     * @param baud_rate Baud rate
     * @return true if successful, false otherwise
     */
    bool (*uart_init)(uint32_t baud_rate);
    
    /**
     * @brief Send character via UART
     * @param c Character to send
     */
    void (*uart_putc)(char c);
    
    /**
     * @brief Get character from UART
     * @return int Character received, or -1 if none
     */
    int (*uart_getc)(void);
    
    /**
     * @brief Get BSP name
     * @return const char* BSP name
     */
    const char* (*get_name)(void);
    
    /**
     * @brief Get BSP version
     * @return const char* BSP version
     */
    const char* (*get_version)(void);
} bsp_interface_t;

/**
 * @brief Get the current BSP interface
 * @return const bsp_interface_t* Pointer to BSP interface
 */
const bsp_interface_t* pxGetBSPInterface(void);

/**
 * @brief Set the BSP interface
 * @param pxInterface Pointer to BSP interface
 */
void vSetBSPInterface(const bsp_interface_t* pxInterface);

#ifdef __cplusplus
}
#endif

#endif /* BSP_INTERFACE_H */
