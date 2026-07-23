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
- ---

# Decision 5 – DC Motor Selection

## Engineering Problem

The robot requires a DC gear motor capable of moving the vehicle smoothly while providing sufficient torque for acceleration, turning, and obstacle avoidance.

The selected motor should maintain stable performance throughout the competition without excessive power consumption.

---

## Engineering Requirements

The motor should:

- Produce sufficient torque.
- Maintain stable speed.
- Fit inside the chassis.
- Be mechanically reliable.
- Operate from the selected battery.
- Be compatible with the selected motor driver.
- Support long operating periods.

---

## Alternatives Evaluated

| Motor | Advantages | Disadvantages |
|--------|------------|---------------|
| TT Gear Motor | Very inexpensive, easy to obtain | Plastic gearbox, lower precision, lower durability |
| N20 Gear Motor | Compact size, good precision | Lower torque for this chassis |
| RS380 Motor | Higher speed, larger power | Larger size, requires different mounting system |
| JGA25-370 Gear Motor | Metal gearbox, compact design, suitable torque, reliable performance | Slightly heavier than smaller motors |

---

## Final Decision

The JGA25-370 geared DC motor was selected because it provides a good balance between torque, size, efficiency, reliability, and compatibility with the selected chassis.

Its integrated metal gearbox produces sufficient torque for autonomous navigation while maintaining a compact mechanical design.

The selected motor also fits directly into the chassis without requiring additional mechanical modifications.

---

## Engineering Justification

The project requires accurate low-speed movement rather than maximum speed.

A geared motor provides higher torque at lower wheel speeds, improving vehicle stability during steering, obstacle avoidance, and cornering.

The gearbox reduction ratio also reduces the mechanical load on the motor, improving efficiency and controllability.

---

## Trade-offs

Advantages

- High torque
- Metal gearbox
- Compact construction
- Reliable operation
- Easy installation
- Good compatibility with the chassis

Disadvantages

- Lower maximum speed than direct-drive motors
- Slightly higher weight
- Gearbox introduces small mechanical losses

- ---

# Decision 6 – Camera Selection

## Engineering Problem

The robot relies on computer vision to detect lanes, walls, colored objects, and obstacles.

The selected camera must provide reliable image quality while maintaining low latency to support real-time autonomous navigation.

---

## Engineering Requirements

The camera should:

- Be compatible with the Raspberry Pi.
- Produce stable image quality.
- Support OpenCV.
- Operate with low latency.
- Be lightweight.
- Be mechanically easy to mount.
- Consume low power.

---

## Alternatives Evaluated

| Camera | Advantages | Disadvantages |
|--------|------------|---------------|
| USB Webcam | Easy to connect, inexpensive, widely available | Higher latency, larger size, increased USB bandwidth usage |
| Smartphone Camera | Excellent image quality | Difficult integration, unstable connection, unsuitable for autonomous robotics |
| Raspberry Pi Camera Module | Native CSI interface, low latency, compact size, excellent OpenCV compatibility | Slightly higher cost than entry-level USB cameras |

---

## Final Decision

The Raspberry Pi Camera Module was selected because it communicates directly through the Raspberry Pi CSI interface.

This provides lower latency, higher stability, and better integration with OpenCV compared to standard USB webcams.

The compact size also simplifies mechanical installation while reducing additional cable management.

---

## Engineering Justification

Computer vision is the most important sensing system in our robot.

Reducing image latency improves steering response, lane detection accuracy, and overall navigation performance.

The Raspberry Pi Camera Module provides direct hardware integration with the Raspberry Pi, allowing image acquisition without unnecessary USB communication overhead.

---

## Trade-offs

Advantages

- Low latency
- Compact design
- Excellent Raspberry Pi compatibility
- Reliable OpenCV support
- Low power consumption

Disadvantages

- Requires CSI ribbon cable
- Dedicated to Raspberry Pi systems
- ---

# Decision 7 – Distance Sensor Selection

## Engineering Problem

The robot requires reliable distance measurements to detect walls, maintain a safe distance, and assist autonomous navigation during the competition.

The selected sensor should provide accurate measurements while remaining compact and easy to integrate into the robot.

---

## Engineering Requirements

The distance sensor should:

- Provide accurate distance measurements.
- Have fast response time.
- Be compact.
- Consume low power.
- Be compatible with the ESP32.
- Operate reliably indoors.
- Support continuous measurements.

---

## Alternatives Evaluated

| Sensor | Advantages | Disadvantages |
|--------|------------|---------------|
| HC-SR04 Ultrasonic | Very inexpensive, easy to use, widely available | Lower accuracy, wider detection angle, affected by surface reflections |
| Sharp IR Distance Sensor | Simple interface, compact size | Lower accuracy at longer distances, affected by object color |
| VL53L1X Time-of-Flight | Laser-based measurement, high accuracy, compact size, fast response | Default I²C address conflict when multiple sensors are connected |

---

## Final Decision

The VL53L1X Time-of-Flight sensor was selected because it provides significantly higher accuracy and faster response than traditional ultrasonic sensors.

Its compact size and laser-based distance measurement make it well suited for precise wall detection during autonomous driving.

---

## Engineering Justification

Reliable wall detection is essential for maintaining vehicle position and avoiding collisions.

Compared to ultrasonic sensors, the VL53L1X offers more stable measurements and is less affected by surrounding objects or irregular wall surfaces.

This improves navigation accuracy during the competition.

---

## Engineering Challenges

During development, both VL53L1X sensors initially shared the same default I²C address.

This prevented both sensors from operating simultaneously on the same I²C bus.

Several software solutions were investigated, including changing the sensor address during startup.

Although one sensor operated correctly, additional debugging was required before both sensors could be used simultaneously.

This challenge and the attempted solutions were documented as part of the engineering development process.

---

## Trade-offs

Advantages

- High measurement accuracy
- Fast response time
- Compact design
- Low power consumption
- Excellent wall detection performance

Disadvantages

- I²C address conflict when using multiple sensors
- More complex software initialization than ultrasonic sensors

- ---

# Decision 8 – Color Sensor Selection

## Engineering Problem

The robot must reliably identify colored objects and field markers under different lighting conditions.

The selected sensor should provide stable color measurements while minimizing the influence of ambient light.

---

## Engineering Requirements

The color sensor should:

- Detect colors accurately.
- Operate under different lighting conditions.
- Be compact and lightweight.
- Be compatible with the ESP32.
- Consume low power.
- Provide stable measurements.
- Be easy to calibrate.

---

## Alternatives Evaluated

| Sensor | Advantages | Disadvantages |
|--------|------------|---------------|
| TCRT5000 | Very inexpensive, simple interface | Suitable only for line detection, cannot accurately distinguish colors |
| Camera-Based Color Detection | High flexibility, detects multiple colors simultaneously | Higher computational load and affected by lighting conditions |
| TCS34725 Color Sensor | Dedicated RGB sensor, integrated white LED, high accuracy, I²C communication | Requires calibration for different lighting environments |

---

## Final Decision

The TCS34725 color sensor was selected because it provides reliable RGB color measurements while remaining compact and easy to integrate with the ESP32.

Its dedicated color sensing hardware allows accurate color recognition without increasing the computational load on the Raspberry Pi.

---

## Engineering Justification

Separating color detection from computer vision reduces the processing workload on the Raspberry Pi and allows color measurements to be performed independently.

This improves software modularity and enables the Raspberry Pi to dedicate more processing power to lane detection, wall detection, and navigation.

---

## Trade-offs

Advantages

- Accurate RGB measurements
- Compact design
- Low power consumption
- Easy integration with ESP32
- Fast measurements

Disadvantages

- Requires calibration under different lighting conditions
- I²C communication requires correct bus configuration

- ---

# Decision 9 – Battery Selection

## Engineering Problem

The robot requires a lightweight and rechargeable power source capable of supplying stable voltage and sufficient current for all electronic components during the competition.

---

## Engineering Requirements

The battery should:

- Supply enough current for the motor and electronics.
- Provide stable voltage.
- Be rechargeable.
- Have good energy density.
- Be lightweight.
- Support long operating time.

---

## Alternatives Evaluated

| Battery | Advantages | Disadvantages |
|---------|------------|---------------|
| 9V Alkaline Battery | Cheap and easy to find | Very low current capability, unsuitable for robotics |
| LiPo 2S | High discharge current, lightweight | Requires careful charging and storage |
| 18650 Li-ion (2S) | Rechargeable, high capacity, widely available, safer handling | Requires battery management and protection |

---

## Final Decision

A 2S Lithium-Ion battery pack was selected because it provides an excellent balance between capacity, weight, operating time, safety, and cost.

The battery voltage is suitable for the drive motor while a DC-DC Buck Converter provides a regulated 5V supply for the Raspberry Pi, ESP32, camera, and sensors.

---

## Trade-offs

Advantages

- Rechargeable
- Lightweight
- High capacity
- Stable output voltage
- Good operating time

Disadvantages

- Requires a charging circuit
- Requires battery protection

- ---

# Decision 10 – Chassis Selection

## Engineering Problem

The robot requires a rigid chassis capable of supporting all mechanical and electronic components while maintaining accurate steering and stable vehicle dynamics.

---

## Engineering Requirements

The chassis should:

- Be mechanically rigid.
- Support rear-wheel drive.
- Provide accurate steering.
- Be lightweight.
- Allow easy maintenance.
- Provide enough space for electronics.

---

## Alternatives Evaluated

| Chassis | Advantages | Disadvantages |
|---------|------------|---------------|
| Acrylic Chassis | Lightweight, inexpensive | Lower mechanical strength |
| 4WD Chassis | Excellent traction | More complex control and unnecessary for this project |
| Rear-Wheel Drive Metal Chassis | Strong, compact, accurate steering, easy maintenance | Slightly heavier than acrylic |

---

## Final Decision

A rear-wheel-drive metal chassis was selected because it provides a rigid mechanical structure, accurate steering, sufficient installation space, and reliable performance during autonomous navigation.

---

## Trade-offs

Advantages

- Strong structure
- Accurate steering
- Easy maintenance
- Good component accessibility

Disadvantages

- Slightly heavier
- Longer assembly time

- ---

# Decision 11 – Communication Protocol

## Engineering Problem

The Raspberry Pi and ESP32 must exchange navigation commands quickly and reliably during autonomous operation.

---

## Alternatives Evaluated

| Protocol | Advantages | Disadvantages |
|----------|------------|---------------|
| I²C | Simple wiring | Master-slave limitations and shared bus complexity |
| SPI | Very high speed | More wiring and unnecessary for this application |
| UART | Simple implementation, reliable communication, supported by both controllers | Point-to-point communication only |

---

## Final Decision

UART serial communication was selected because it provides a simple, reliable, and efficient communication channel between the Raspberry Pi and ESP32 while minimizing software complexity.

---

## Trade-offs

Advantages

- Reliable
- Simple implementation
- Fast enough for control commands

Disadvantages

- Point-to-point communication
- Limited scalability compared to network protocols

- ---

# Decision 12 – Software Architecture

## Engineering Decision

The software was divided into independent modules instead of implementing a single large program.

This modular architecture improves code readability, simplifies debugging, enables independent testing, and allows future features to be added without affecting the entire project.

Each module has a clearly defined responsibility, improving maintainability and overall software quality.

---

## Advantages

- Modular code
- Easier debugging
- Easier testing
- Better scalability
- Improved maintainability

---

## Disadvantages

- Slightly larger project structure
- More communication between modules

- ---

# Risk Analysis

During the development of the robot, several potential risks were identified. For each risk, a mitigation strategy was planned to improve the overall reliability and safety of the system.

| Risk | Possible Impact | Mitigation Strategy |
|------|-----------------|---------------------|
| Raspberry Pi freezes | Robot stops processing camera images | ESP32 will stop the motor if no command is received within a predefined timeout period. |
| UART communication failure | Loss of communication between Raspberry Pi and ESP32 | ESP32 enters a safe stop mode until communication is restored. |
| Low battery voltage | Unstable operation or Raspberry Pi reboot | A regulated 5V buck converter supplies the electronics, and the battery voltage is monitored during testing. |
| Servo current spikes | Voltage drops affecting other electronics | The servo is powered from a regulated power supply capable of handling peak current. |
| Camera malfunction | Lane and obstacle detection become unavailable | The robot immediately stops instead of continuing with invalid navigation data. |
| Distance sensor failure | Incorrect wall distance measurements | Sensor values are validated in software, and unexpected readings are ignored when possible. |
| Motor driver overheating | Reduced motor performance or shutdown | The motor driver is operated within its rated limits and monitored during testing. |

## Engineering Perspective

Identifying possible failure scenarios before the competition helps improve the reliability, maintainability, and safety of the robot. Risk analysis was considered an important part of the engineering design process rather than an afterthought.
