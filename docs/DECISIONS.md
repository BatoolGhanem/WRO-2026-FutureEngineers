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
- ---

# Decision 3 – Motor Driver Selection

## Engineering Problem

The DC motor requires a dedicated driver capable of controlling both speed and direction while safely handling the motor current.

The motor driver must also interface easily with the ESP32 and operate reliably throughout long testing sessions.

---

## Engineering Requirements

The motor driver should:

- Support bidirectional motor control.
- Operate from the selected battery voltage.
- Be compatible with ESP32 logic levels.
- Provide stable operation.
- Be easy to replace if future upgrades become necessary.
- Have sufficient current capability.

---

## Alternatives Evaluated

| Driver | Advantages | Disadvantages |
|--------|------------|---------------|
| L9110S | Small, inexpensive, simple wiring | Low current capability, unsuitable for larger DC motors |
| TB6612FNG | High efficiency, low voltage drop, compact size | Slightly more expensive and less available locally |
| BTS7960 | Very high current capability, excellent efficiency | Larger than required for our current motor |
| L298N | Widely available, inexpensive, simple to use, well documented | Lower efficiency and noticeable voltage drop |

---

## Final Decision

The current prototype uses the **L298N motor driver**.

The main reasons for selecting this driver were its availability, ease of integration, extensive documentation, and compatibility with our motor and battery configuration.

Although newer drivers provide higher efficiency, the L298N was considered sufficient during the development stage and allowed rapid hardware integration and software testing.

---

## Future Improvements

Future versions of the robot may replace the L298N with a more efficient motor driver such as the TB6612FNG.

This would reduce voltage loss, improve motor efficiency, decrease heat generation, and increase the available voltage at the motor terminals.

---

## Trade-offs

Advantages

- Easy to obtain.
- Simple software interface.
- Large amount of educational documentation.
- Reliable during development.

Disadvantages

- Higher voltage drop than modern MOSFET drivers.
- Lower electrical efficiency.
- Generates more heat under higher loads.
- ---

# Decision 4 – Steering Servo Selection

## Engineering Problem

The steering system requires a servo motor capable of accurately controlling the front wheels while overcoming the steering resistance generated by the tires and steering mechanism.

A weak servo could result in inaccurate steering, slow response, or failure to reach the required steering angle.

---

## Engineering Requirements

The steering servo should:

- Provide sufficient torque.
- Have good positioning accuracy.
- Be mechanically reliable.
- Support standard PWM control.
- Operate continuously during long testing sessions.
- Be compatible with the ESP32.

---

## Alternatives Evaluated

| Servo | Advantages | Disadvantages |
|--------|------------|---------------|
| SG90 | Small, inexpensive, low power consumption | Very low torque, plastic gears, unsuitable for steering |
| MG90S | Metal gears, better precision than SG90 | Torque still limited for our steering mechanism |
| DS3218 | Very high torque, waterproof versions available | Larger size, heavier, more expensive |
| MG996R | High torque, metal gears, reliable, widely available | Higher current consumption and larger size |

---

## Final Decision

The MG996R servo was selected because it provides sufficient torque to steer the vehicle reliably while maintaining accurate steering control.

Its metal gear construction improves durability compared to smaller plastic gear servos, making it more suitable for repeated testing and competition use.

---

## Why We Selected This Servo

The steering mechanism transfers significant mechanical load to the servo during turning.

Selecting a high-torque servo reduces steering errors, improves repeatability, and increases the reliability of autonomous navigation.

Although the MG996R consumes more current than smaller servos, the additional torque provides a significant improvement in steering performance.

---

## Trade-offs

Advantages

- High torque
- Metal gears
- Reliable operation
- Excellent availability
- Easy PWM control

Disadvantages

- Higher current consumption
- Heavier than micro servos
- Larger physical dimensions
