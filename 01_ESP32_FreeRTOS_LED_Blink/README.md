# Experiment 01: ESP32 FreeRTOS LED Blink

## Project Overview
This experiment represents the foundational step in the **RTOS Journey**, transitioning from the classic single-threaded "Super Loop" bare-metal programming paradigm to a preemptive, multi-tasking Real-Time Operating System (**FreeRTOS**) environment on the Espressif **ESP32-WROOM-32** platform.

In standard microcontroller programming, delays are commonly implemented using busy-wait polling loops (e.g., `delay()`), which block the processor core from doing any productive work. Under FreeRTOS, tasks yield execution back to the kernel scheduler when waiting, enabling non-blocking execution, high CPU efficiency, and deterministic timing.

![ESP32 FreeRTOS Architecture](../assets/esp32_freertos_architecture.svg)

---

## Objective
- Create and launch a standalone FreeRTOS task using the native FreeRTOS API (`xTaskCreate`).
- Demonstrate non-blocking task execution and periodic timing using `vTaskDelay()` and `pdMS_TO_TICKS()`.
- Observe the task lifecycle: **Running** $\to$ **Blocked** $\to$ **Ready** $\to$ **Running**.
- Understand how the Arduino-ESP32 runtime itself operates as an underlying FreeRTOS task (`loopTask`).

---

## Hardware Specifications
| Component | Specification / Details |
| :--- | :--- |
| **Microcontroller Board** | ESP32-WROOM-32 Development Board (30-pin / 38-pin) |
| **Processor Silicon** | Xtensa Dual-Core 32-bit LX6 Microprocessor @ 240 MHz |
| **Internal Memory** | 520 KB SRAM |
| **External Memory** | 4 MB Quad-SPI Flash |
| **Actuator Indicator** | Onboard Blue SMD LED connected to **GPIO 2** |
| **Interconnect** | Micro-USB / USB-C to USB-A programming & serial cable |

---

## Software & Toolchain
- **Framework**: Arduino Core for ESP32 (v2.x / v3.x)
- **Kernel**: FreeRTOS for ESP-IDF (SMP dual-core support)
- **Headers**: `<Arduino.h>`, `"freertos/FreeRTOS.h"`, `"freertos/task.h"`
- **IDE**: Arduino IDE 2.x / PlatformIO / VS Code + Arduino Extension
- **Baud Rate**: 115200 bps

---

## Implementation Details

The implementation in [`led_blink_esp32_wroom.ino`](./led_blink_esp32_wroom.ino) leverages core FreeRTOS primitives:

### 1. Task Handle Definition
```c
TaskHandle_t ledTaskHandle = NULL;
```
A handle variable `ledTaskHandle` of type `TaskHandle_t` is instantiated globally to store reference data for the created task. This handle allows external tasks or ISRs to query status, suspend, or delete the task if needed.

### 2. Task Function Implementation
```c
void toggleLED(void *parameter)
{
    Serial.println("LED Task Started!");

    while (true)
    {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON");
        vTaskDelay(pdMS_TO_TICKS(500));

        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```
- **Signature**: Must adhere to `void FunctionName(void *parameter)`.
- **Infinite Loop**: FreeRTOS tasks must never return from their top-level function unless explicitly destroyed via `vTaskDelete(NULL)`. Returning without deleting leads to a kernel panic.
- **`vTaskDelay(pdMS_TO_TICKS(ms))`**: Puts the calling task into the **Blocked** state for exactly 500 milliseconds (based on the kernel tick rate `configTICK_RATE_HZ`, typically 1000 Hz / 1 ms per tick on ESP32). During this time, the CPU core is liberated to run other ready tasks or the FreeRTOS Idle Task.

### 3. Task Creation in `setup()`
```c
BaseType_t result = xTaskCreate(
    toggleLED,        // Function pointer to task entry routine
    "LED_Task",       // Human-readable task identifier (for debugging)
    2048,             // Stack depth allocated in bytes (on ESP32)
    NULL,             // Parameter passed into task function
    1,                // Task Priority (1 = low, higher numbers = higher priority)
    &ledTaskHandle    // Pointer to handle variable
);
```
- **Stack Allocation**: `2048` bytes are allocated from internal SRAM for `toggleLED`'s stack frame, register spills, and local variable storage.
- **Priority**: Assigned priority `1`, running above the Idle Task (priority 0) and at parity with `loopTask`.
- **Error Validation**: `result == pdPASS` confirms successful memory allocation and scheduler registration.

### 4. Background `loop()` Operation
```c
void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}
```
In Arduino-ESP32, `setup()` and `loop()` are executed inside `loopTask` created automatically during system boot on Core 1 with priority 1. Yielding in `loop()` with `vTaskDelay()` prevents starvation and unnecessary CPU burning.

---

## Execution Flow & Lifecycle

```
[System Boot & ESP-IDF Init]
            │
            ▼
[FreeRTOS Kernel Launch] ──► Spawns loopTask (Priority 1, Core 1)
            │
            ▼
        setup()
            ├─► Serial.begin(115200) & pinMode(GPIO 2, OUTPUT)
            ├─► xTaskCreate(toggleLED, "LED_Task", 2048, NULL, 1, &ledTaskHandle)
            └─► Returns control
            │
            ▼
 ┌────────────────────────────────────────────────────────────────────────┐
 │                      FreeRTOS Preemptive Scheduler                     │
 └────────────────────────────────────────────────────────────────────────┘
            │
            ▼ (Dispatches highest priority Ready task)
┌─────────────────────────────────────────────────────────────────────────┐
│ LED_Task Cycle:                                                         │
│ 1. [RUNNING]  digitalWrite(2, HIGH) ──► LED Illuminates                 │
│ 2. [RUNNING]  vTaskDelay(500ms) invoked                                 │
│ 3. [BLOCKED]  Removed from Ready list; placed into Delayed Task List    │
│               Scheduler context-switches to loopTask or IDLE Task       │
│ 4. [READY]    SysTick ISR detects 500 ms elapsed; moves task to Ready   │
│ 5. [RUNNING]  Scheduler selects LED_Task; context restored              │
│ 6. [RUNNING]  digitalWrite(2, LOW) ──► LED Extinguishes                 │
│ 7. [BLOCKED]  vTaskDelay(500ms) invoked; repeats indefinitely           │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## Technical Concepts Learned

1. **Super Loop vs. Real-Time Multi-Tasking**:
   - Super Loop executes sequentially in a rigid loop. If one sensor read or peripheral waits, the whole system stalls.
   - RTOS provides preemptive multithreading where tasks release the CPU cooperatively or are preempted by priority.

2. **Arduino Core under FreeRTOS**:
   - On the ESP32, Arduino runs as a secondary layer on top of ESP-IDF FreeRTOS. The runtime initializes the kernel and starts `loopTask` before executing `setup()`.

3. **Task States and FSM**:
   - **Running**: Active on the physical CPU core.
   - **Blocked**: Waiting for a temporal event (`vTaskDelay`) or synchronization primitive (Queue, Semaphore).
   - **Ready**: Prepared for execution, awaiting scheduler dispatch.
   - **Suspended**: Manually paused until explicit resumption.

4. **Context Switching & Stack Integrity**:
   - When switching out of `LED_Task`, the CPU registers (PC, SP, A0-A15) are saved onto the task stack.
   - Sizing stacks properly (e.g., 2048 bytes) prevents catastrophic stack overflows.

---

## Expected Output

### Physical Board
- The blue onboard LED connected to GPIO 2 alternates ON and OFF at a constant 1 Hz frequency (500 ms ON, 500 ms OFF, 50% duty cycle).

### Serial Monitor (`115200 baud`)
```text
Task Created Successfully!
LED Task Started!
LED ON
LED OFF
LED ON
LED OFF
...
```

---

## Build and Run Instructions

1. **Open Project**:
   - Launch **Arduino IDE**.
   - Open [`led_blink_esp32_wroom.ino`](./led_blink_esp32_wroom.ino).

2. **Board Configuration**:
   - Navigate to **Tools $\to$ Board $\to$ ESP32 Arduino $\to$ ESP32 Dev Module** (or your specific ESP32-WROOM board model).
   - Set **Upload Speed**: `921600` or `115200`.
   - Set **CPU Frequency**: `240MHz (WiFi/BT)`.
   - Set **Flash Frequency**: `80MHz`.

3. **Port Selection**:
   - Navigate to **Tools $\to$ Port** and select your active USB-to-UART bridge COM port (e.g., `COM3`, `COM4`, `/dev/ttyUSB0`).

4. **Compile and Flash**:
   - Click **Verify** (`Ctrl+R`) to compile the FreeRTOS sketch.
   - Click **Upload** (`Ctrl+U`) to flash the firmware onto the ESP32.
   - *(Note: If the upload hangs at `Connecting........_____`, press and hold the `BOOT` button on the ESP32 board until flashing begins).*

5. **Observe Telemetry**:
   - Open the **Serial Monitor** (`Ctrl+Shift+M`) set to **115200 baud**.

---

## Future Improvements & Next Steps
- [ ] **Dual Task Concurrency**: Launch two tasks toggling different pins at distinct frequencies to demonstrate preemptive scheduling.
- [ ] **Core Pinning (`xTaskCreatePinnedToCore`)**: Bind tasks explicitly to Core 0 (`PRO_CPU`) and Core 1 (`APP_CPU`).
- [ ] **Priority Preemption Analysis**: Introduce higher-priority tasks to demonstrate execution starvation and preemption.
- [ ] **Inter-Task Communication (Queues)**: Pass state changes and telemetry between tasks using FreeRTOS Queues.
- [ ] **Synchronization Primitives**: Implement Binary Semaphores and Mutexes to guard shared hardware buses (I2C, SPI, UART).
