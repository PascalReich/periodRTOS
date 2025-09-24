# periodRTOS Build Guide

## Modular Build System

The periodRTOS now supports a modular build system that allows easy configuration of different boards and schedulers.

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

## Benefits

- **Easy board addition**: Just add a directory and implement the interface
- **Scheduler flexibility**: Swap schedulers without code changes
- **Automatic detection**: Build system finds available boards and schedulers
- **Modular design**: Clean separation of concerns
- **Future-proof**: Easy to extend with new hardware and algorithms
