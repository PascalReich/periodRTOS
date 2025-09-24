# periodRTOS Build Guide

## Modular Build System

The periodRTOS now supports a modular build system that follows industry standards:
- **Minimal port layer**: Only RTOS-essential hardware functions
- **Optional BSP**: Convenience functions for common peripherals
- **Vendor HAL integration**: Use STM32 HAL, Nordic SDK, ESP-IDF, etc.

## Building with Different Boards

### STM32F3 Discovery (Default)
```bash
mkdir build && cd build
cmake .. -DTARGET_BOARD=stm32f3_discovery
make
```

### STM32F4 Discovery
```bash
mkdir build && cd build
cmake .. -DTARGET_BOARD=stm32f4_discovery
make
```

### RP2040 Pico W (Future)
```bash
mkdir build && cd build
cmake .. -DTARGET_BOARD=rp2040_pico_w
make
```

## Building with Different Schedulers

### Rate Monotonic (Default)
```bash
mkdir build && cd build
cmake .. -DTARGET_SCHEDULER=rm
make
```

### Earliest Deadline First (Future)
```bash
mkdir build && cd build
cmake .. -DTARGET_SCHEDULER=edf
make
```

### Round Robin (Future)
```bash
mkdir build && cd build
cmake .. -DTARGET_SCHEDULER=rr
make
```

## Combined Configuration

```bash
mkdir build && cd build
cmake .. -DTARGET_BOARD=stm32f4_discovery -DTARGET_SCHEDULER=rm
make
```

## Available Boards

- `stm32f3_discovery` - STM32F3 Discovery board
- `stm32f4_discovery` - STM32F4 Discovery board
- `rp2040_pico_w` - Raspberry Pi Pico W (planned)

## Available Schedulers

- `rm` - Rate Monotonic scheduler
- `edf` - Earliest Deadline First scheduler (planned)
- `rr` - Round Robin scheduler (planned)

## Adding New Boards

1. Create a new directory under `boards/` (e.g., `boards/my_board/`)
2. Add the following files:
   - `board_init.c` - Board initialization code
   - `init.c` - Reset handler and startup code
   - `my_board_FLASH.ld` - Linker script
   - `startup_my_board.s` - Assembly startup code (optional)
3. The build system will automatically detect and configure the board

## Adding New Schedulers

1. Create a new scheduler plugin file (e.g., `src/scheduler/my_scheduler_plugin.c`)
2. Implement the `scheduler_interface_t` structure
3. Add the scheduler to `cmake/scheduler_config.cmake`
4. The build system will automatically include the scheduler

## Configuration Files

- `cmake/board_config.cmake` - Board-specific configuration
- `cmake/scheduler_config.cmake` - Scheduler-specific configuration
- `CMakeLists.txt` - Main build configuration

## Architecture Benefits

### **Industry Standard Approach**
- **Minimal port layer**: Only RTOS-essential functions (like FreeRTOS)
- **Vendor HAL integration**: Use existing STM32 HAL, Nordic SDK, etc.
- **Optional BSP**: Convenience functions for common peripherals
- **Clean separation**: RTOS core is hardware-agnostic

### **Easy Integration**
- **Leverage existing code**: Use vendor HALs and examples
- **Familiar patterns**: Matches FreeRTOS, Zephyr approach
- **Less maintenance**: Don't reinvent peripheral drivers
- **Better compatibility**: Works with existing ecosystems

### **Modular Design**
- **Easy board addition**: Just implement port layer interface
- **Scheduler flexibility**: Swap schedulers without code changes
- **Automatic detection**: Build system finds available boards and schedulers
- **Future-proof**: Easy to extend with new hardware and algorithms

## Usage Examples

### **Using STM32 HAL (Recommended)**
```c
#include "periodRTOS.h"
#include "stm32f3xx_hal.h"

void vTask1(void *pvParameters) {
    // Use STM32 HAL for peripherals
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    
    // Use periodRTOS for task management
    vTaskDelay(500);
}
```

### **Using periodRTOS BSP (Optional)**
```c
#include "periodRTOS.h"

void vTask2(void *pvParameters) {
    // Use periodRTOS BSP for convenience
    vBSPLedOn(0);
    vTaskDelay(200);
    vBSPLedOff(0);
}
```
