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

## Engineering Justification

Separating high-level decision making from low-level control reduces software complexity and minimizes the impact of CPU-intensive image processing on steering and motor response.

This architecture also makes the project easier to debug, maintain, and extend during future development.
