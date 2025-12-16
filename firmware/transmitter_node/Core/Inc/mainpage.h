/**
 * @mainpage Reversing Radar Distance Monitoring System
 *
 * @section intro_sec Introduction
 * This project implements a **Reversing Radar system** using two STM32F103-based nodes:
 * a **Transmitter Node** and a **Receiver Node**. The system measures distances to obstacles 
 * behind a vehicle using ultrasonic sensors and provides feedback via LEDs, a buzzer, and 
 * a 2-digit 7-segment display. CAN bus is used for communication between the two nodes.
 *
 * The software is developed with **STM32Cube HAL** and **FreeRTOS** for task scheduling.
 *
 * @section hw_sec Hardware Overview
 * The following components are used in this project:
 * - **MCU:** STM32F103C8T6 (ARM Cortex-M3)
 * - **CAN Transceivers:** MCP2551
 * - **Ultrasonic Sensors:** HC-SR04 or equivalent
 * - **Buzzer:** Generic active buzzer
 * - **LEDs:** Red, Green, Blue standard 5mm LEDs
 * - **7-segment Display:** 2281AS 2-digit common anode 7-segment display
 *
 * @section sw_sec Software Structure
 * - **Transmitter Node**
 *   - Measures distances from ultrasonic sensors.
 *   - Sends measured distances via CAN bus.
 *   - Generates FreeRTOS tasks for sensor reading and CAN transmission.
 *
 * - **Receiver Node**
 *   - Receives CAN messages containing distance data.
 *   - Controls LEDs, buzzer, and 7-segment display based on distance thresholds.
 *   - Implements FreeRTOS tasks for display updates, buzzer, and UART debug output.
 *
 * @section usage_sec Usage
 * 1. Compile the project in Keil uVision for each node.
 * 2. Flash the firmware to the respective STM32F103 boards.
 * 3. Connect CAN bus between the two nodes.
 * 4. Power the system. The receiver node will display distance and activate 
 *    LEDs/buzzer according to obstacle proximity.
 *
 * @section note_sec Notes
 * - The project uses symbolic names for GPIO pins and peripherals (e.g., `Buzzer_Pin`, `DIG1_Pin`).
 * - All hardware references in this documentation use actual component names in the Hardware Overview section.
 */
