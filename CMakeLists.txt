cmake_minimum_required(VERSION 3.16)
project(periodRTOS VERSION 1.0.0 LANGUAGES C ASM)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include configuration modules
include(cmake/board_config.cmake)
include(cmake/scheduler_config.cmake)

# Check if we're cross-compiling
if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling for ${TARGET_BOARD}")
else()
    message(STATUS "Native compilation")
endif()

# Set compiler flags from board configuration
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BOARD_MCU_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${BOARD_MCU_FLAGS}")

# Add board and scheduler defines
add_definitions(${BOARD_DEFINES})
add_definitions(${SCHEDULER_DEFINES})

# Optimization and debug flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --save-temps")

# Include directories
include_directories(include)

# Core kernel sources (hardware-agnostic)
set(KERNEL_SOURCES
    src/kernel/kernel.c
    src/kernel/context_switch.S
    src/timer/systick.c
    src/monitor/monitor.c
    src/hal/syscalls.c
)

# Add scheduler sources from configuration
list(APPEND KERNEL_SOURCES ${SCHEDULER_SOURCES})

# Add board-specific HAL sources
list(APPEND KERNEL_SOURCES ${BOARD_HAL_SOURCES})

# Create kernel library
add_library(periodRTOS_kernel STATIC ${KERNEL_SOURCES})

# Create board library
add_library(periodRTOS_board STATIC ${BOARD_SOURCES})

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
#    -Map system.map
    -Wl,--gc-sections
    -Wl,--print-memory-usage
    -save-temps=obj
)

target_link_options(example_app
  PUBLIC
    LINKER:-Map=foo.map
)



# Set output directory
set_target_properties(example_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Print build information
message(STATUS "Building periodRTOS for ${TARGET_BOARD}")
message(STATUS "Scheduler: ${TARGET_SCHEDULER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
