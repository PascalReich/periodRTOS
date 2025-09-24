/**
 * @file port_interface.h
 * @brief Minimal port layer interface for periodRTOS
 * 
 * This interface provides only the essential hardware functions needed
 * for the RTOS to operate. Applications should use vendor HALs for
 * peripheral access (STM32 HAL, Nordic SDK, ESP-IDF, etc.)
 */

#ifndef PORT_INTERFACE_H
#define PORT_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NULL 0

/**
 * @brief Port layer interface structure
 * 
 * This interface contains only the minimal functions required for
 * RTOS operation. All other hardware access should be done through
 * vendor HALs in the application layer.
 */
typedef struct {
    /**
     * @brief Initialize the port layer
     * 
     * This function should perform minimal system initialization
     * required for RTOS operation (disable watchdog, set up basic
     * system clocks, etc.)
     */
    void (*init)(void);
    
    /**
     * @brief Initialize system timer for RTOS tick
     * 
     * @param tick_freq_hz Desired tick frequency in Hz
     * @return true if successful, false otherwise
     */
    bool (*timer_init)(uint32_t tick_freq_hz);
    
    /**
     * @brief Get current tick count
     * 
     * @return uint32_t Current tick count
     */
    uint32_t (*get_tick_count)(void);
    
    /**
     * @brief Perform context switch
     * 
     * This function should save the current task context and
     * restore the next task context.
     */
    void (*context_switch)(void);
    
    /**
     * @brief Task yield
     * 
     * This function should trigger a context switch to the
     * next ready task.
     */
    void (*yield)(void);
    
    /**
     * @brief Disable interrupts
     * 
     * @return uint32_t Previous interrupt state (for restore)
     */
    uint32_t (*disable_interrupts)(void);
    
    /**
     * @brief Enable interrupts
     * 
     * @param state Previous interrupt state from disable_interrupts
     */
    void (*enable_interrupts)(uint32_t state);
    
    /**
     * @brief Get system core clock frequency
     * 
     * @return uint32_t System clock frequency in Hz
     */
    uint32_t (*get_system_clock)(void);
    
    /**
     * @brief Get port name
     * 
     * @return const char* Port name (e.g., "STM32F3", "RP2040")
     */
    const char* (*get_name)(void);
    
    /**
     * @brief Get port version
     * 
     * @return const char* Port version
     */
    const char* (*get_version)(void);
} port_interface_t;

/**
 * @brief Get the current port interface
 * 
 * @return const port_interface_t* Pointer to port interface
 */
const port_interface_t* pxGetPortInterface(void);

/**
 * @brief Set the port interface
 * 
 * @param pxInterface Pointer to port interface
 */
void vSetPortInterface(const port_interface_t* pxInterface);

#ifdef __cplusplus
}
#endif

#endif /* PORT_INTERFACE_H */
