/**
 * @file basic_periodic_tasks.c
 * @brief Example application demonstrating periodic tasks with periodRTOS
 */

#include "periodRTOS.h"
#include "stm32f303xx.h"

#define NULL 0

/* Task handles */
static TaskHandle_t xTask1Handle = NULL;
static TaskHandle_t xTask2Handle = NULL;
static TaskHandle_t xTask3Handle = NULL;

/* Task function prototypes */
static void vTask1(void *pvParameters);
static void vTask2(void *pvParameters);
static void vTask3(void *pvParameters);
//static void vIdleTask(void *pvParameters);

/**
 * @brief Task 1 - High frequency task (100ms period)
 */
static void vTask1(void *pvParameters)
{
    uint32_t ulTask1Counter = 0;
    
    while (1) {
        /* Task 1 work - toggle LED 0 */
        vLedToggle(0);
        
        /* Simulate some work */
        for (volatile int i = 0; i < 1000; i++);
        
        ulTask1Counter++;
        
        /* Yield to other tasks */
        vTaskYield();
        
        /* Wait for next period */
        vTaskDelay(100);
    }
}

/**
 * @brief Task 2 - Medium frequency task (500ms period)
 */
static void vTask2(void *pvParameters)
{
    uint32_t ulTask2Counter = 0;
    
    while (1) {
        /* Task 2 work - toggle LED 1 */
        vLedToggle(1);
        
        /* Simulate some work */
        for (volatile int i = 0; i < 2000; i++);
        
        ulTask2Counter++;
        
        /* Yield to other tasks */
        vTaskYield();
        
        /* Wait for next period */
        vTaskDelay(500);
    }
}

/**
 * @brief Task 3 - Low frequency task (1000ms period)
 */
static void vTask3(void *pvParameters)
{
    uint32_t ulTask3Counter = 0;
    char pcBuffer[256];
    
    while (1) {
        /* Task 3 work - toggle LED 2 */
        vLedToggle(2);
        
        /* Simulate some work */
        for (volatile int i = 0; i < 3000; i++);
        
        ulTask3Counter++;
        
        /* Print task information every 10 iterations */
        if (ulTask3Counter % 10 == 0) {
            //vGetTaskInfo(xTask1Handle, pcBuffer, sizeof(pcBuffer));
            /* In a real system, this would be sent via UART */
            
            //vGetTaskInfo(xTask2Handle, pcBuffer, sizeof(pcBuffer));
            /* In a real system, this would be sent via UART */
            
            //vGetTaskInfo(xTask3Handle, pcBuffer, sizeof(pcBuffer));
            /* In a real system, this would be sent via UART */
        }
        
        /* Yield to other tasks */
        vTaskYield();
        
        /* Wait for next period */
        vTaskDelay(1000);
    }
}

/**
 * @brief Idle task - runs when no other tasks are ready
 */
void vIdleTask(void *pvParameters)
{
    while (1) {
        /* Idle task work - toggle LED 3 */
        vLedToggle(3);
        
        /* Simulate idle work */
        //for (volatile int i = 0; i < 100; i++);
        
        /* Yield to other tasks */
        vTaskYield();
        
        /* Small delay to prevent busy waiting */
        vTaskDelay(10);
    }
}

/**
 * @brief Main function
 */
int main(void)
{
    /* Initialize board */
    vBoardInit();
    
    /* Create periodic tasks */
    xTask1Handle = xTaskCreatePeriodic(vTask1, "Task1", 
                                      512, NULL, 
                                      100, 80);  /* 100ms period, 80ms deadline */
    
    xTask2Handle = xTaskCreatePeriodic(vTask2, "Task2", 
                                      512, NULL, 
                                      500, 400); /* 500ms period, 400ms deadline */
    
    xTask3Handle = xTaskCreatePeriodic(vTask3, "Task3", 
                                      512, NULL, 
                                      1000, 800); /* 1000ms period, 800ms deadline */
    
    /* Check if tasks were created successfully */
    if (xTask1Handle == NULL || xTask2Handle == NULL || xTask3Handle == NULL) {
        /* Error creating tasks - blink all LEDs */
        while (1) {
            for (int i = 0; i < 7; i++) {
                vLedOn(i);
            }
            for (volatile int i = 0; i < 1000000; i++);
            for (int i = 0; i < 7; i++) {
                vLedOff(i);
            }
            for (volatile int i = 0; i < 1000000; i++);
        }
    }
    
    /* Start the scheduler */
    vTaskStartScheduler();
    
    /* Should never reach here */
    while (1) {
        /* Error - scheduler failed to start */
        for (int i = 0; i < 7; i++) {
            vLedOn(i);
        }
    }
}
