/**
 * @file scheduler_interface.h
 * @brief Scheduler plugin interface for periodRTOS
 */

#ifndef SCHEDULER_INTERFACE_H
#define SCHEDULER_INTERFACE_H

#include "periodRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Scheduler plugin interface structure
 */
typedef struct {
    /**
     * @brief Initialize the scheduler
     */
    void (*init)(void);
    
    /**
     * @brief Get the next task to run
     * @return TaskHandle_t Next task to run, or NULL if no task ready
     */
    TaskHandle_t (*get_next_task)(void);
    
    /**
     * @brief Add a task to the scheduler
     * @param xTask Task handle to add
     */
    void (*add_task)(TaskHandle_t xTask);
    
    /**
     * @brief Remove a task from the scheduler
     * @param xTask Task handle to remove
     */
    void (*remove_task)(TaskHandle_t xTask);
    
    /**
     * @brief Update task priorities (if applicable)
     */
    void (*update_priorities)(void);
    
    /**
     * @brief System tick handler
     */
    void (*tick_handler)(void);
    
    /**
     * @brief Get scheduler name
     * @return const char* Scheduler name
     */
    const char* (*get_name)(void);
    
    /**
     * @brief Get scheduler version
     * @return const char* Scheduler version
     */
    const char* (*get_version)(void);
} scheduler_interface_t;

/**
 * @brief Get the current scheduler interface
 * @return const scheduler_interface_t* Pointer to scheduler interface
 */
const scheduler_interface_t* pxGetSchedulerInterface(void);

/**
 * @brief Set the scheduler interface
 * @param pxInterface Pointer to scheduler interface
 */
void vSetSchedulerInterface(const scheduler_interface_t* pxInterface);

#ifdef __cplusplus
}
#endif

#endif /* SCHEDULER_INTERFACE_H */
