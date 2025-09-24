/**
 * @file vendor_hal_example.c
 * @brief Example showing how to use vendor HALs with periodRTOS
 * 
 * This example demonstrates the recommended approach:
 * - Use periodRTOS port layer for RTOS-essential functions
 * - Use vendor HAL (STM32 HAL) for peripheral access
 * - Use periodRTOS BSP for convenience functions (optional)
 */

#include "periodRTOS.h"
#include "stm32f303xx.h"  /* STM32 HAL includes */

/* STM32 HAL includes */
#include "stm32f3xx_hal.h"

/* Global variables */
UART_HandleTypeDef huart1;
GPIO_InitTypeDef GPIO_InitStruct;

/**
 * @brief Task 1: Blink LED using STM32 HAL
 */
void vTask1(void *pvParameters)
{
    (void)pvParameters;
    
    /* Configure GPIO using STM32 HAL */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    while (1) {
        /* Use STM32 HAL for GPIO operations */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        vTaskDelay(500);  /* periodRTOS delay function */
        
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        vTaskDelay(500);
    }
}

/**
 * @brief Task 2: Send UART data using STM32 HAL
 */
void vTask2(void *pvParameters)
{
    (void)pvParameters;
    
    /* Configure UART using STM32 HAL */
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* Configure GPIO for UART */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* Configure UART */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
    
    const char* message = "Hello from periodRTOS with STM32 HAL!\r\n";
    
    while (1) {
        /* Use STM32 HAL for UART operations */
        HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
        vTaskDelay(1000);  /* periodRTOS delay function */
    }
}

/**
 * @brief Task 3: Use periodRTOS BSP for convenience
 */
void vTask3(void *pvParameters)
{
    (void)pvParameters;
    
    uint32_t led = 0;
    
    while (1) {
        /* Use periodRTOS BSP for LED control (convenience) */
        vBSPLedOn(led);
        vTaskDelay(200);
        
        vBSPLedOff(led);
        led = (led + 1) % 8;  /* Cycle through LEDs 0-7 */
        
        vTaskDelay(200);
    }
}

/**
 * @brief Main function
 */
int main(void)
{
    /* Initialize periodRTOS */
    vBoardInit();
    
    /* Create tasks */
    xTaskCreatePeriodic(vTask1, "Task1", 256, NULL, 1000, 100);
    xTaskCreatePeriodic(vTask2, "Task2", 512, NULL, 2000, 200);
    xTaskCreatePeriodic(vTask3, "Task3", 256, NULL, 500, 50);
    
    /* Start scheduler */
    vTaskStartScheduler();
    
    /* Should never reach here */
    while (1) {
        /* Error handling */
    }
}

/**
 * @brief STM32 HAL error handler
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* Handle UART errors */
    (void)huart;
}

/**
 * @brief STM32 HAL MSP init
 */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    /* STM32 HAL MSP initialization */
    (void)huart;
}

/**
 * @brief STM32 HAL MSP deinit
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    /* STM32 HAL MSP deinitialization */
    (void)huart;
}
