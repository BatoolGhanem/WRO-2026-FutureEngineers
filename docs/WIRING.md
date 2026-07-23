# Electrical Wiring

The robot is built around an ESP32 DevKit V1 mounted on an ESP32 Expansion Shield, which serves as the main controller for all sensors and actuators. A Raspberry Pi 3 Model B operates as a companion computer for vision processing and communicates with the ESP32 through a UART connection. The Raspberry Pi Camera is connected directly to the Raspberry Pi using the CSI interface.

The power system is supplied by a 2S Li-ion battery. Battery voltage is delivered directly to the L298N motor driver and to an XL4015 DC-DC buck converter. The buck converter is adjusted to provide a stable 5.06 V output, which powers both the ESP32 through its VIN pin and the MG996R steering servo. For safety, the main power switch is installed only on the battery ground (negative) line. All components share a common electrical ground.

The drivetrain consists of a DC drive motor controlled by an L298N motor driver. Motor speed is controlled using PWM through GPIO25, while GPIO26 and GPIO27 determine the motor direction. Steering is performed using an MG996R servo driven by a PWM signal from GPIO13.

Two independent I²C buses are used to improve communication reliability. The first bus (GPIO21 SDA, GPIO22 SCL) connects the two VL53L1X Time-of-Flight distance sensors and the MPU6050 inertial measurement unit. Since both VL53L1X sensors have the same default I²C address, their XSHUT pins are connected to GPIO18 and GPIO19, allowing each sensor to be initialized individually with a unique address. The second I²C bus (GPIO16 SDA, GPIO17 SCL) is dedicated exclusively to the TCS34725 color sensor.

The complete electrical architecture, including power distribution, communication buses, control signals, and GPIO assignments, is illustrated in Figure 1.

![Electrical Wiring Diagram](images/wiring.png)

**Figure 1.** Electrical wiring and communication architecture of the robot, showing the power distribution, communication interfaces, GPIO assignments, sensors, actuators, and common ground connections.
