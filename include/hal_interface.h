/**
 * @file hal_interface.h
 * @brief Hardware Abstraction Layer interface for periodRTOS
 */

#ifndef HAL_INTERFACE_H
#define HAL_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief HAL interface structure
 */
typedef struct {
    /**
     * @brief Initialize the hardware abstraction layer
     */
    void (*init)(void);
    
    /**
     * @brief Configure system clock
     */
    void (*configure_clock)(void);
    
    /**
     * @brief Initialize GPIO
     */
    void (*init_gpio)(void);
    
    /**
     * @brief Initialize system timer
     */
    void (*init_timer)(void);
    
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
     * @brief Set interrupt priority
     * @param irq Interrupt number
     * @param priority Priority level
     */
    void (*nvic_set_priority)(int irq, uint32_t priority);
    
    /**
     * @brief Enable interrupt
     * @param irq Interrupt number
     */
    void (*nvic_enable_irq)(int irq);
    
    /**
     * @brief Disable interrupt
     * @param irq Interrupt number
     */
    void (*nvic_disable_irq)(int irq);
    
    /**
     * @brief Get system core clock frequency
     * @return uint32_t Clock frequency in Hz
     */
    uint32_t (*get_system_clock)(void);
    
    /**
     * @brief System initialization
     */
    void (*system_init)(void);
    
    /**
     * @brief Get HAL name
     * @return const char* HAL name
     */
    const char* (*get_name)(void);
    
    /**
     * @brief Get HAL version
     * @return const char* HAL version
     */
    const char* (*get_version)(void);
} hal_interface_t;

/**
 * @brief Get the current HAL interface
 * @return const hal_interface_t* Pointer to HAL interface
 */
const hal_interface_t* pxGetHALInterface(void);

/**
 * @brief Set the HAL interface
 * @param pxInterface Pointer to HAL interface
 */
void vSetHALInterface(const hal_interface_t* pxInterface);

#ifdef __cplusplus
}
#endif

#endif /* HAL_INTERFACE_H */
