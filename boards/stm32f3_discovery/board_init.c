/**
 * @file board_init.c
 * @brief STM32F3 (e.g., STM32F303 Discovery) board initialization
 */

#include "periodRTOS.h"
#include "port_interface.h"
#include "bsp_interface.h"

/**
 * @brief Initialize STM32F3 Discovery board
 */
void vBoardInit(void)
{
    /* Initialize port layer */
#ifdef STM32F3
    extern const port_interface_t* pxGetSTM32F3PortInterface(void);
    vSetPortInterface(pxGetSTM32F3PortInterface());
#endif
    
    /* Initialize port layer */
    vPortInit();
    
    /* Initialize Systick */
    vSystickInit();
    
    /* Initialize kernel */
    vKernelInit();
    
    /* Initialize BSP (optional) */
#ifdef STM32F3
    extern const bsp_interface_t* pxGetSTM32F3DiscoveryBSPInterface(void);
    vSetBSPInterface(pxGetSTM32F3DiscoveryBSPInterface());
    vBSPInit();
#endif
}

/* Board-specific functions are now handled by the HAL interface */
