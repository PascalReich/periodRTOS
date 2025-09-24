# Scheduler configuration for periodRTOS
# This file handles scheduler plugin selection and configuration

# Set default scheduler if not specified
if(NOT DEFINED TARGET_SCHEDULER)
    set(TARGET_SCHEDULER "rm" CACHE STRING "Target scheduler")
endif()

message(STATUS "Configuring scheduler: ${TARGET_SCHEDULER}")

# Available schedulers
set(AVAILABLE_SCHEDULERS "rm" "edf" "rr")

# Validate scheduler selection
if(NOT ${TARGET_SCHEDULER} IN_LIST AVAILABLE_SCHEDULERS)
    message(FATAL_ERROR "Scheduler '${TARGET_SCHEDULER}' not found. Available schedulers: ${AVAILABLE_SCHEDULERS}")
endif()

# Scheduler-specific sources and defines
if(TARGET_SCHEDULER STREQUAL "rm")
    set(SCHEDULER_SOURCES 
        "src/scheduler/scheduler_manager.c"
        "src/scheduler/rm_scheduler_plugin.c"
    )
    set(SCHEDULER_DEFINES "-DSCHEDULER_RM")
elseif(TARGET_SCHEDULER STREQUAL "edf")
    set(SCHEDULER_SOURCES 
        "src/scheduler/scheduler_manager.c"
        "src/scheduler/edf_scheduler_plugin.c"
    )
    set(SCHEDULER_DEFINES "-DSCHEDULER_EDF")
elseif(TARGET_SCHEDULER STREQUAL "rr")
    set(SCHEDULER_SOURCES 
        "src/scheduler/scheduler_manager.c"
        "src/scheduler/rr_scheduler_plugin.c"
    )
    set(SCHEDULER_DEFINES "-DSCHEDULER_RR")
endif()

message(STATUS "Scheduler sources: ${SCHEDULER_SOURCES}")
message(STATUS "Scheduler defines: ${SCHEDULER_DEFINES}")
