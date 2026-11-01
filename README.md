# RTOS Journey

> Practical exploration of Real-Time Operating Systems, task scheduling, synchronization, memory management and embedded firmware development.

---

## Overview

Welcome to **RTOS Journey** — a repository documenting hands-on embedded firmware development using **FreeRTOS** on real-time microcontrollers. The goal of this repository is to systematically progress from foundational multi-tasking and deterministic scheduling to advanced inter-task communication (IPC), concurrency management, resource arbitration, and real-time hardware peripheral control.

All experiments are engineered with clean code practices, production-minded stack and memory configurations, comprehensive technical documentation, and vector architectural diagrams.

---

## Architectural Foundation

FreeRTOS operates as a preemptive, priority-driven real-time kernel. On the Espressif **ESP32-WROOM-32**, the FreeRTOS implementation runs on dual Xtensa LX6 cores with Symmetric Multiprocessing (SMP) support:

![ESP32 FreeRTOS System Architecture](./assets/esp32_freertos_architecture.svg)

---

## Hardware Platforms & Toolchain

### Target Hardware
- **Primary Platform**: Espressif ESP32-WROOM-32 (Xtensa Dual-Core 32-bit LX6 @ 240 MHz, 520 KB SRAM, 4 MB Flash)
- **Secondary Platforms** (Roadmap): STM32 ARM Cortex-M series, Raspberry Pi RP2040

### Software & Frameworks
- **Kernel**: FreeRTOS Kernel (SMP port)
- **Framework**: Arduino-ESP32 / ESP-IDF Core
- **Development Environments**: Arduino IDE 2.x, PlatformIO, VS Code
- **Serial Debugging**: 115200 bps UART Telemetry

---

## Experiment Repository Index

| # | Experiment Name | Focus Area | Status | Directory Link |
| :-: | :--- | :--- | :-: | :--- |
| **01** | **ESP32 FreeRTOS LED Blink** | Task Creation, Preemption, State FSM, `vTaskDelay()` | Completed | [01_ESP32_FreeRTOS_LED_Blink](./01_ESP32_FreeRTOS_LED_Blink/) |
| **02** | **Native ESP-IDF Multiple Tasks** | Dual tasks, ESP-IDF drivers, `app_main()`, independent scheduling | Completed | [02_ESP32_Native_ESP_IDF_Multiple_Tasks](./02_ESP32_Native_ESP_IDF_Multiple_Tasks/) |
| **03** | **Task Priorities & Preemption** | Priority inversions, starvation, tick-based time slicing | Planned | *Upcoming* |
| **04** | **Task Synchronization** | Semaphores, Mutexes, critical sections | Planned | *Upcoming* |
| **05** | **Queues & Inter-Task Communication** | Safe cross-task message passing, FIFO buffers, telemetry | Planned | *Upcoming* |
| **06** | **Binary & Counting Semaphores** | Hardware interrupt synchronization (ISR to Task handoff) | Planned | *Upcoming* |
| **07** | **Mutexes & Shared Resources** | Critical section guarding, Priority Inheritance | Planned | *Upcoming* |
| **08** | **Software Timers** | One-shot and auto-reload timers, task health monitoring | Planned | *Upcoming* |
| **09** | **Interrupts & ISR** | GPIO interrupts, ISR design, deferred processing | Planned | *Upcoming* |
| **10** | **Memory Management** | Stack analysis, heap management, memory optimization | Planned | *Upcoming* |

---

## Completed Experiments

### [Experiment 01: ESP32 FreeRTOS LED Blink](./01_ESP32_FreeRTOS_LED_Blink/)
- **Objective**: Transition from blocking bare-metal delays (`delay()`) to non-blocking preemptive FreeRTOS tasks.
- **Key Concepts**: `xTaskCreate()`, task stack sizing (2048 bytes), task priorities, task finite state machine (Running, Blocked, Ready), `vTaskDelay()`, `pdMS_TO_TICKS()`, and Arduino core's `loopTask` relationship.
- **Hardware Validated**: ESP32-WROOM-32 onboard blue LED on GPIO 2 blinking at a deterministic 1 Hz rate.
- **Framework**: Arduino-ESP32 Core

### [Experiment 02: Native ESP-IDF Multiple FreeRTOS Tasks](./02_ESP32_Native_ESP_IDF_Multiple_Tasks/)
- **Objective**: Transition from Arduino framework to native ESP-IDF firmware development with dual independently scheduled FreeRTOS tasks.
- **Key Concepts**: `app_main()` entry point, ESP-IDF GPIO driver (`gpio_set_level()`), ESP logging system (`ESP_LOGI()`), dual-task concurrency, preemptive scheduling, task state lifecycle, native FreeRTOS API access.
- **Hardware Validated**: ESP32-WROOM-32 with GPIO2 LED toggling at 1Hz and independent serial monitoring at 1000ms intervals.
- **Framework**: Native ESP-IDF v6.1

---

## Repository Structure

```text
RTOS-Journey/
│
├── README.md                                 # Root repository overview and syllabus
├── LICENSE                                   # MIT License
│
├── assets/
│   ├── esp32_freertos_architecture.svg       # Vector diagram of FreeRTOS task & memory layers
│   ├── esp32_task_scheduler.svg              # Native ESP-IDF dual-task architecture
│   ├── task_lifecycle.svg                    # FreeRTOS task state machine
│   ├── execution_timeline.svg               # Task execution timing diagram
│   └── arduino_vs_esp_idf.svg                # Framework comparison diagram
│
├── 01_ESP32_FreeRTOS_LED_Blink/              # Experiment 01: Arduino framework
│   ├── README.md                             # In-depth technical breakdown and execution guide
│   └── led_blink_esp32_wroom.ino             # Production-commented Arduino/FreeRTOS source
│
└── 02_ESP32_Native_ESP_IDF_Multiple_Tasks/   # Experiment 02: Native ESP-IDF
    ├── README.md                             # Comprehensive ESP-IDF documentation
    ├── CMakeLists.txt                        # Project build configuration
    └── main/
        ├── CMakeLists.txt                    # Component registration
        └── main.c                            # Native ESP-IDF source code
```

---

## Roadmap

- [x] Experiment 01: Single-Task FreeRTOS LED Blinker with State Transitions (Arduino Framework)
- [x] Experiment 02: Native ESP-IDF Multiple FreeRTOS Tasks with Independent Scheduling
- [ ] Experiment 03: Task Priority Dynamics, Preemption & Starvation Analysis
- [ ] Experiment 04: Task Synchronization with Semaphores and Mutexes
- [ ] Experiment 05: Inter-Task Communication via FreeRTOS Queues
- [ ] Experiment 06: External GPIO Interrupts synchronized via Binary Semaphores
- [ ] Experiment 07: Software Timer Service Daemons & Watchdog Implementation
- [ ] Experiment 08: GPIO Interrupts and ISR Design
- [ ] Experiment 09: Memory Management and Stack Analysis
- [ ] Experiment 10: Advanced FreeRTOS Patterns and Optimization

---

## Author & Maintainer

**Priyadharsan D**
- GitHub: [@PRIY4DH4RS4N-D](https://github.com/PRIY4DH4RS4N-D)
- Focus: Embedded Systems, Real-Time Operating Systems (RTOS), Firmware Engineering

---

## License

This project is licensed under the [MIT License](./LICENSE). Feel free to use, modify, and reference for embedded systems learning and development.
