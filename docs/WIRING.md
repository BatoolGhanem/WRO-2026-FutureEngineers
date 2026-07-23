# Electrical Wiring

The robot is built around an ESP32 DevKit V1 mounted on an ESP32 Expansion Shield, which serves as the main controller for all sensors and actuators. A Raspberry Pi 3 Model B acts as a companion computer for vision processing and communicates with the ESP32 through a UART connection. The Raspberry Pi Camera is connected directly to the Raspberry Pi using the CSI interface.

The power system is supplied by a 2S Li-ion battery. Battery voltage is delivered directly to the L298N motor driver and to an XL4015 DC-DC buck converter. The buck converter is adjusted to provide a stable 5.06 V output, which powers both the ESP32 through its VIN pin and the MG996R steering servo. The main power switch is installed on the battery ground (negative) line, and all components share a common electrical ground.

Two independent I²C buses are used in the system. The first bus (GPIO21 SDA, GPIO22 SCL) connects the two VL53L1X distance sensors together with the MPU6050 IMU. Since both VL53L1X sensors have the same default I²C address, their XSHUT pins are connected to GPIO18 and GPIO19, allowing each sensor to be initialized individually with a unique address. The second I²C bus (GPIO16 SDA, GPIO17 SCL) is dedicated exclusively to the TCS34725 color sensor.

The complete electrical architecture of the robot is illustrated below.

![Electrical Wiring Diagram](images/electronics/wiring.png)

**Figure 1.** Electrical wiring and communication architecture of the robot.

---

## Power Distribution

| Source | Destination |
|---------|-------------|
| 2S Li-ion Battery (+) | L298N +12V |
| 2S Li-ion Battery (+) | XL4015 IN+ |
| Battery GND | Main Power Switch |
| Main Power Switch | L298N GND |
| Main Power Switch | XL4015 IN− |
| XL4015 Output (5.06 V) | ESP32 VIN |
| XL4015 Output (5.06 V) | MG996R Servo |

---

## Main Controllers

| Device | Function |
|--------|----------|
| ESP32 DevKit V1 | Main controller for sensors and actuators |
| Raspberry Pi 3 Model B | Companion computer for vision processing |
| Raspberry Pi Camera | Connected to Raspberry Pi through CSI |

---

## Motor and Steering Control

| Device | Connection |
|--------|------------|
| L298N ENA | GPIO25 |
| L298N IN1 | GPIO26 |
| L298N IN2 | GPIO27 |
| MG996R Steering Servo | GPIO13 (PWM) |

---

## Sensor Connections

| Sensor | Interface | GPIO |
|--------|-----------|------|
| VL53L1X Left | I²C Bus 1 | SDA GPIO21 / SCL GPIO22 / XSHUT GPIO18 |
| VL53L1X Right | I²C Bus 1 | SDA GPIO21 / SCL GPIO22 / XSHUT GPIO19 |
| MPU6050 IMU | I²C Bus 1 | SDA GPIO21 / SCL GPIO22 |
| TCS34725 Color Sensor | I²C Bus 2 | SDA GPIO16 / SCL GPIO17 |

---

## GPIO Assignment

| GPIO | Function |
|------|----------|
| GPIO13 | MG996R Servo PWM |
| GPIO16 | I²C Bus 2 SDA |
| GPIO17 | I²C Bus 2 SCL |
| GPIO18 | VL53L1X Left XSHUT |
| GPIO19 | VL53L1X Right XSHUT |
| GPIO21 | I²C Bus 1 SDA |
| GPIO22 | I²C Bus 1 SCL |
| GPIO25 | L298N ENA (PWM) |
| GPIO26 | L298N IN1 |
| GPIO27 | L298N IN2 |

---

## Communication Interfaces

| Interface | Connected Devices |
|-----------|-------------------|
| UART | Raspberry Pi ↔ ESP32 |
| I²C Bus 1 | VL53L1X Left, VL53L1X Right, MPU6050 |
| I²C Bus 2 | TCS34725 |
| CSI | Raspberry Pi Camera ↔ Raspberry Pi |

The wiring architecture was designed to provide reliable power distribution, organized communication buses, and clear separation between sensing, control, and actuation subsystems while maintaining a common ground throughout the robot.
