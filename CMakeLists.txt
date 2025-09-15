cmake_minimum_required(VERSION 3.16)
project(periodRTOS VERSION 1.0.0 LANGUAGES C ASM)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Check if we're cross-compiling
if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling for ARM Cortex-M4")
else()
    message(STATUS "Native compilation")
endif()

# Compiler flags for ARM Cortex-M4
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MCU_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${MCU_FLAGS}")

# Optimization and debug flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# Include directories
include_directories(include)

# Source files
set(KERNEL_SOURCES
    src/kernel/kernel.c
    src/kernel/context_switch.S
    src/scheduler/rm_scheduler.c
#    src/tasks/task_manager.c
    src/timer/systick.c
    src/monitor/monitor.c
    src/hal/stm32_hal.c
    src/hal/syscalls.c
)

# Board-specific sources
set(BOARD_SOURCES
    boards/stm32f4_discovery/board_init.c
    #boards/stm32f4_discovery/startup_stm32f407xx.s
    boards/stm32f4_discovery/init.c
)

# Create kernel library
add_library(periodRTOS_kernel STATIC ${KERNEL_SOURCES})

# Create board library
add_library(periodRTOS_board STATIC ${BOARD_SOURCES})

# Linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/boards/stm32f4_discovery/STM32F407VGTx_FLASH.ld)

# Example application
add_executable(example_app
    examples/basic_periodic_tasks.c
)

target_link_libraries(example_app 
    periodRTOS_kernel 
    periodRTOS_board
)

target_link_options(example_app PRIVATE 
    -T ${LINKER_SCRIPT}
    -Wl,--gc-sections
    -Wl,--print-memory-usage
)

# Set output directory
set_target_properties(example_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Print build information
message(STATUS "Building periodRTOS for ARM Cortex-M4")
message(STATUS "Target: STM32F4 Discovery")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
