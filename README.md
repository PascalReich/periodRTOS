# periodRTOS

A periodic task-focused Real-Time Operating System (RTOS) designed for embedded systems, with built-in Rate Monotonic scheduling and real-time guarantees.

## Features

- **Periodic Task Model**: Tasks are created with specific periods and deadlines
- **Rate Monotonic Scheduling**: Automatic priority assignment based on task periods
- **Real-Time Guarantees**: Deadline miss detection and monitoring
- **ARM Cortex-M4 Support**: Optimized for STM32F3 Discovery board (STM32F303 microcontroller)
- **Monitoring System**: Context switch counting, execution time tracking, deadline miss detection
- **Static Memory Allocation**: Fixed stack sizes for predictable memory usage
- **FreeRTOS-like API**: Familiar interface for embedded developers

## Architecture

### Core Components

1. **Kernel** (`src/kernel/`): Core RTOS functionality, task management, context switching
2. **Scheduler** (`src/scheduler/`): Rate Monotonic scheduling algorithm
3. **Timer** (`src/timer/`): Systick timer integration for system tick
4. **Monitor** (`src/monitor/`): System monitoring and debugging
5. **Board Support** (`boards/stm32f3_discovery/`): STM32F3 Discovery board initialization

### Task Model

- **Periodic Tasks**: Created with period and deadline parameters
- **Idle Task**: Runs when no other tasks are ready
- **Task States**: READY, RUNNING, BLOCKED, SUSPENDED, DELETED
- **Priority Assignment**: Automatic based on Rate Monotonic algorithm (shorter period = higher priority)

## API Reference

### Task Management

```c
// Create a periodic task
TaskHandle_t xTaskCreatePeriodic(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                uint32_t ulStackSize,
                                void *pvParameters,
                                uint32_t ulPeriod,
                                uint32_t ulDeadline);

// Start the scheduler
void vTaskStartScheduler(void);

// Yield current task
void vTaskYield(void);

// Suspend/Resume tasks
void vTaskSuspend(TaskHandle_t xTask);
void vTaskResume(TaskHandle_t xTask);
```

### Monitoring

```c
// Get monitoring data
uint32_t ulGetContextSwitchCount(void);
uint32_t ulGetTaskExecutionTime(TaskHandle_t xTask);
uint32_t ulGetDeadlineMissCount(TaskHandle_t xTask);
bool bIsTaskDeadlineMissed(TaskHandle_t xTask);
SystemMonitor_t* pxGetSystemMonitor(void);
```

### Timing

```c
// System timing
uint32_t ulGetSystemTick(void);

// Note: vTaskDelay and vTaskDelayUntil functions are deprecated
// and will be removed in future versions in favor of instance-based task model
```

## Configuration

### System Configuration

- **Maximum Tasks**: 12
- **Maximum Priority Levels**: 12
- **Default Stack Size**: 512 bytes
- **System Tick Frequency**: 1000 Hz (1ms)
- **Target Architecture**: ARM Cortex-M4

### Board Configuration

- **Target Board**: STM32F3 Discovery
- **Microcontroller**: STM32F303VCT6
- **System Clock**: 72 MHz (HSI-based)
- **Flash**: 256 KB
- **RAM**: 48 KB

## Building

### Prerequisites

- ARM GCC toolchain
- CMake 3.16 or later
- STM32F3 Discovery board

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Example Application

The example application (`examples/basic_periodic_tasks.c`) demonstrates:

- Creating three periodic tasks with different periods (100ms, 500ms, 1000ms)
- LED blinking to visualize task execution
- Task monitoring and information display
- Rate Monotonic scheduling in action

## Rate Monotonic Scheduling

The RTOS implements the Rate Monotonic scheduling algorithm:

- **Priority Assignment**: Tasks with shorter periods get higher priorities
- **Schedulability**: The system can be analyzed for schedulability using Liu & Layland theorem
- **Deadline Miss Detection**: Automatic detection and counting of deadline misses
- **Real-Time Guarantees**: Predictable timing behavior for periodic tasks

## Monitoring and Debugging

The monitoring system provides:

- **Context Switch Counting**: Track system activity
- **Execution Time Tracking**: Monitor task execution times
- **Deadline Miss Detection**: Identify timing violations
- **System Utilization**: Calculate CPU utilization
- **Task Information**: Detailed task state and timing information

## Planned Features (TODO)

### High Priority
- [ ] **Synchronization/IPC**: Implement mutexes, semaphores, and message queues for inter-task communication
- [ ] **Instance-based Task Model**: Transition from cyclic task model to task instances for better real-time guarantees
- [ ] **Remove Task Delay Functions**: Eliminate vTaskDelay and vTaskDelayUntil in favor of proper periodic task scheduling
- [ ] **Deadline Miss Monitoring**: Enhanced monitoring and reporting of deadline violations
- [ ] **Task Phasing**: Support for task phase offsets to improve schedulability

### Medium Priority
- [ ] **Raspberry Pi Pico W Support**: Port to RP2040 microcontroller with WiFi capabilities
- [ ] **Memory Protection**: Add memory protection features for enhanced safety
- [ ] **Additional Scheduling Algorithms**: Implement EDF (Earliest Deadline First) and other scheduling policies
- [ ] **Schedulability Analysis Tools**: Built-in tools for analyzing system schedulability

### Low Priority
- [ ] **Multi-core Support**: Support for symmetric multiprocessing
- [ ] **More Board Support**: Additional STM32 and ARM Cortex-M boards
- [ ] **Power Management**: Low-power modes and dynamic frequency scaling

## License

This project is open source. See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## Acknowledgments

- Inspired by FreeRTOS design principles
- Based on Rate Monotonic scheduling theory by Liu & Layland
- Designed for educational and research purposes
