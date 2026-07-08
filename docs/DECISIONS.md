# Engineering Decisions

## Introduction

Every major decision in this project was made after comparing multiple alternatives instead of selecting components based on popularity or personal preference.

For each important subsystem, we identified the engineering requirements, evaluated the available options, discussed their advantages and disadvantages, and finally selected the solution that best matched the objectives of our robot.

This document explains the reasoning behind every important engineering decision made during the project.

---

# Decision 1 – System Controller Selection

## Engineering Problem

The robot must process camera images, detect lanes and walls, recognize colors, control steering, drive the motor, read multiple sensors, and make navigation decisions in real time.

Using a single controller for all these tasks could reduce performance, increase software complexity, and make debugging more difficult.

---

## Engineering Requirements

The controller architecture should:

- Support computer vision.
- Provide reliable real-time control.
- Be easy to debug.
- Allow future expansion.
- Support communication between software modules.
- Keep the software modular.

---

## Alternatives Evaluated

| Controller | Advantages | Disadvantages |
|------------|------------|---------------|
| Arduino Uno | Very simple, inexpensive, huge community support | Insufficient RAM and processing power for computer vision and multiple concurrent tasks |
| Arduino Mega | More I/O pins than Uno, simple programming | Still not suitable for OpenCV or image processing |
| ESP32 | Excellent real-time performance, Wi-Fi, Bluetooth, low power consumption | Cannot efficiently perform computer vision |
| Raspberry Pi 3B | Powerful processor, Linux operating system, OpenCV support, USB and CSI camera support | Real-time motor control is less deterministic under heavy CPU load |
| Raspberry Pi 3B + ESP32 | Combines powerful image processing with reliable real-time control | Slightly more complex hardware and software architecture |

---

## Final Decision

Our robot uses both a Raspberry Pi 3B and an ESP32.

The Raspberry Pi is responsible for all high-level processing tasks including:

- Camera image acquisition
- Lane detection
- Wall detection
- Color recognition
- Navigation logic
- Decision making

The ESP32 is responsible for all low-level control tasks including:

- Steering servo control
- DC motor control
- Reading sensors
- Executing movement commands
- Safety functions

Communication between both controllers is performed using UART serial communication.

---

## Why We Selected This Architecture

Separating computer vision from low-level motor control improves system reliability and software organization.

The Raspberry Pi can dedicate all processing power to image analysis while the ESP32 guarantees stable real-time control regardless of processor load.

This architecture also simplifies debugging and future upgrades because each controller has a clearly defined responsibility.

---

## Trade-offs

Advantages

- Excellent modularity
- Better real-time performance
- Easier debugging
- Better scalability
- Improved software organization

Disadvantages

- Two controllers instead of one
- Additional UART communication required
- Slightly more complex software architecture

---

# Decision 2 – Power System Selection

## Engineering Problem

The robot requires a stable power system capable of supplying multiple electronic devices with different voltage and current requirements.

The Raspberry Pi, ESP32, sensors, servo motor, and DC motor cannot all operate directly from the same voltage without proper power regulation.

Therefore, a reliable and efficient power distribution system was required.

---

## Engineering Requirements

The power system should:

- Provide stable power for all electronic components.
- Supply sufficient current during peak loads.
- Support long testing sessions.
- Be lightweight.
- Be easy to recharge.
- Minimize voltage drop.
- Improve overall system reliability.

---

## Alternatives Evaluated

| Power Source | Advantages | Disadvantages |
|--------------|------------|---------------|
| 9V Alkaline Battery | Low cost, widely available | Very low current capability, unsuitable for motors and Raspberry Pi |
| 18650 Li-ion (2S) | Rechargeable, lightweight, high energy density, easy to replace | Requires protection circuit and charger |
| LiPo Battery (2S) | High discharge current, lightweight, widely used in robotics | Requires careful charging and handling |

---

## Battery Selection

After evaluating the available options, we selected a **2S Lithium-Ion battery pack (7.4V)**.

This configuration provides sufficient voltage for the motor while allowing the remaining electronics to be powered through a voltage regulator.

The selected battery also provides a good balance between runtime, weight, cost, and availability.

---

## Voltage Regulation

Different components require different supply voltages.

The DC motor operates from the battery voltage through the motor driver.

The Raspberry Pi, ESP32, camera, and sensors require a stable 5V supply.

For this reason, a DC-DC Buck Converter is used to convert the battery voltage into a regulated 5V output.

---

## Alternatives for Voltage Regulation

| Converter | Advantages | Disadvantages |
|------------|------------|---------------|
| Linear Voltage Regulator | Simple design | Low efficiency, high heat generation |
| Boost Converter | Useful when increasing voltage | Cannot reduce battery voltage |
| DC-DC Buck Converter | High efficiency, low heat, stable output | Slightly higher cost |

---

## Final Decision

The robot will use:

- 2S 7.4V Lithium-Ion Battery
- DC-DC Buck Converter for regulated 5V
- Shared common ground between all controllers

This configuration provides stable operation while reducing power losses and improving reliability.

---

## Trade-offs

Advantages

- High efficiency
- Stable power distribution
- Long runtime
- Rechargeable
- Suitable for future expansion

Disadvantages

- Additional converter required
- Slightly more complex wiring
