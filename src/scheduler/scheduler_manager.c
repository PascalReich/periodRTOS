/**
 * @file scheduler_manager.c
 * @brief Scheduler plugin manager for periodRTOS
 */

#include "scheduler_interface.h"
#include <string.h>

/* Current scheduler interface */
static const scheduler_interface_t* pxCurrentScheduler = NULL;

/**
 * @brief Get the current scheduler interface
 */
const scheduler_interface_t* pxGetSchedulerInterface(void)
{
    return pxCurrentScheduler;
}

/**
 * @brief Set the scheduler interface
 */
void vSetSchedulerInterface(const scheduler_interface_t* pxInterface)
{
    if (pxInterface != NULL) {
        pxCurrentScheduler = pxInterface;
    }
}

/**
 * @brief Initialize the scheduler (wrapper)
 */
void vSchedulerInit(void)
{
    if (pxCurrentScheduler != NULL && pxCurrentScheduler->init != NULL) {
        pxCurrentScheduler->init();
    }
}

/**
 * @brief Get the next task to run (wrapper)
 */
TaskHandle_t vSchedulerGetNextTask(void)
{
    if (pxCurrentScheduler != NULL && pxCurrentScheduler->get_next_task != NULL) {
        return pxCurrentScheduler->get_next_task();
    }
    return NULL;
}

/**
 * @brief Add task to scheduler (wrapper)
 */
void vSchedulerAddTask(TaskHandle_t xTask)
{
    if (pxCurrentScheduler != NULL && pxCurrentScheduler->add_task != NULL) {
        pxCurrentScheduler->add_task(xTask);
    }
}

/**
 * @brief Remove task from scheduler (wrapper)
 */
void vSchedulerRemoveTask(TaskHandle_t xTask)
{
    if (pxCurrentScheduler != NULL && pxCurrentScheduler->remove_task != NULL) {
        pxCurrentScheduler->remove_task(xTask);
    }
}

/**
 * @brief Update task priorities (wrapper)
 */
void vSchedulerUpdatePriorities(void)
{
    if (pxCurrentScheduler != NULL && pxCurrentScheduler->update_priorities != NULL) {
        pxCurrentScheduler->update_priorities();
    }
}

/**
 * @brief System tick handler (wrapper)
 */
void vSchedulerTickHandler(void)
{
    if (pxCurrentScheduler != NULL && pxCurrentScheduler->tick_handler != NULL) {
        pxCurrentScheduler->tick_handler();
    }
}
