# Board configuration for periodRTOS
# This file handles board-specific configuration and auto-detection

# Set default board if not specified
if(NOT DEFINED TARGET_BOARD)
    set(TARGET_BOARD "stm32f3_discovery" CACHE STRING "Target board")
endif()

message(STATUS "Configuring for board: ${TARGET_BOARD}")

# Auto-detect available boards
file(GLOB BOARD_DIRS "boards/*")
set(AVAILABLE_BOARDS "")

foreach(BOARD_DIR ${BOARD_DIRS})
    get_filename_component(BOARD_NAME ${BOARD_DIR} NAME)
    if(IS_DIRECTORY ${BOARD_DIR})
        list(APPEND AVAILABLE_BOARDS ${BOARD_NAME})
    endif()
endforeach()

message(STATUS "Available boards: ${AVAILABLE_BOARDS}")

# Validate board selection
if(NOT ${TARGET_BOARD} IN_LIST AVAILABLE_BOARDS)
    message(FATAL_ERROR "Board '${TARGET_BOARD}' not found. Available boards: ${AVAILABLE_BOARDS}")
endif()

# Set board-specific variables
set(BOARD_DIR "${CMAKE_SOURCE_DIR}/boards/${TARGET_BOARD}")
set(BOARD_SOURCES_DIR "${BOARD_DIR}")

# Auto-detect board source files
set(BOARD_SOURCES "")
if(EXISTS "${BOARD_DIR}/board_init.c")
    list(APPEND BOARD_SOURCES "${BOARD_DIR}/board_init.c")
endif()
if(EXISTS "${BOARD_DIR}/init.c")
    list(APPEND BOARD_SOURCES "${BOARD_DIR}/init.c")
endif()
if(EXISTS "${BOARD_DIR}/startup_${TARGET_BOARD}.s")
    list(APPEND BOARD_SOURCES "${BOARD_DIR}/startup_${TARGET_BOARD}.s")
endif()

# Auto-detect linker script
set(LINKER_SCRIPT "")
if(EXISTS "${BOARD_DIR}/${TARGET_BOARD}_FLASH.ld")
    set(LINKER_SCRIPT "${BOARD_DIR}/${TARGET_BOARD}_FLASH.ld")
elseif(EXISTS "${BOARD_DIR}/linker.ld")
    set(LINKER_SCRIPT "${BOARD_DIR}/linker.ld")
else()
    message(FATAL_ERROR "No linker script found for board ${TARGET_BOARD}")
endif()

# Board-specific compiler flags
if(TARGET_BOARD STREQUAL "stm32f3_discovery")
    set(BOARD_MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
    set(BOARD_DEFINES "-DSTM32F3 -DSTM32F303xC")
elseif(TARGET_BOARD STREQUAL "stm32f4_discovery")
    set(BOARD_MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
    set(BOARD_DEFINES "-DSTM32F4 -DSTM32F407xx")
elseif(TARGET_BOARD STREQUAL "rp2040_pico_w")
    set(BOARD_MCU_FLAGS "-mcpu=cortex-m0plus -mthumb")
    set(BOARD_DEFINES "-DRP2040 -DPICO_W")
else()
    message(WARNING "Unknown board ${TARGET_BOARD}, using default flags")
    set(BOARD_MCU_FLAGS "-mcpu=cortex-m4 -mthumb")
    set(BOARD_DEFINES "")
endif()

# Board-specific HAL
if(TARGET_BOARD STREQUAL "stm32f3_discovery" OR TARGET_BOARD STREQUAL "stm32f4_discovery")
    set(BOARD_HAL_SOURCES "src/hal/stm32_hal.c")
elseif(TARGET_BOARD STREQUAL "rp2040_pico_w")
    set(BOARD_HAL_SOURCES "src/hal/rp2040_hal.c")
else()
    message(FATAL_ERROR "No HAL implementation found for board ${TARGET_BOARD}")
endif()

message(STATUS "Board sources: ${BOARD_SOURCES}")
message(STATUS "Linker script: ${LINKER_SCRIPT}")
message(STATUS "MCU flags: ${BOARD_MCU_FLAGS}")
message(STATUS "Board defines: ${BOARD_DEFINES}")
message(STATUS "HAL sources: ${BOARD_HAL_SOURCES}")
