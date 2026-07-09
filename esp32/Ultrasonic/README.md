# HC-SR04 Ultrasonic Sensor Validation

## Objective

The objective of this test is to validate the HC-SR04 ultrasonic distance sensor before integrating it into the WRO Future Engineers autonomous vehicle.

The ultrasonic sensor is intended to support:

- Front obstacle detection
- Distance estimation
- Collision avoidance
- Safety monitoring

---

# Engineering Decision

The HC-SR04 ultrasonic sensor was used as a temporary prototype during the early development phase.

While the TOF400C-VL53L1X sensors are planned to be the primary distance sensors for the final robot, additional software validation and integration are still under investigation.

To allow continuous development of the obstacle detection algorithms without delaying the project, the HC-SR04 was selected as an initial replacement for testing.

This approach allows the navigation, obstacle avoidance, and control software to be developed and validated independently while work on the TOF sensors continues.

The HC-SR04 will be replaced by the TOF sensors once their initialization and communication have been fully validated.
---

# Hardware

## Components

- ESP32 DevKit V1
- HC-SR04 Ultrasonic Sensor
- Jumper wires

---

# Wiring

| HC-SR04 | ESP32 |
|----------|--------|
| VCC | 5V |
| GND | GND |
| TRIG | GPIO32 |
| ECHO | GPIO33* |

\*During early validation, ECHO was connected directly to the ESP32 for a short functional test. For permanent integration, a voltage divider or logic level shifter should be used because the HC-SR04 outputs a 5V logic signal.

---

# Software Description

The validation program performs the following operations:

1. Generates a 10 µs trigger pulse.
2. Measures the echo pulse duration.
3. Calculates the distance in centimeters.
4. Displays the measured distance through the Serial Monitor.
5. Repeats the measurement continuously.

---

# Test Procedure

1. Connect the sensor according to the wiring table.
2. Upload the validation program.
3. Open the Serial Monitor at **115200 baud**.
4. Place an object at different distances.
5. Verify that the measured distance changes accordingly.

---

# Expected Output

Example Serial Monitor output:

```text
HC-SR04 Ultrasonic Test Started

Distance: 18.4 cm
Distance: 24.7 cm
Distance: 39.2 cm
Distance: 61.8 cm
```

---

# Test Results

The sensor successfully detected nearby objects and continuously reported distance measurements.

The measured values changed consistently as the target object moved closer or farther from the sensor.

---

# Engineering Notes

The HC-SR04 measures distance by transmitting ultrasonic pulses and measuring the echo return time.

The measured pulse duration is converted into distance using the speed of sound.

For final robot integration, the ECHO output will be connected through a voltage divider to ensure compatibility with the ESP32's 3.3V GPIO inputs.

---

# Future Integration

The ultrasonic sensor will later be integrated with:

- Obstacle Avoidance
- Navigation State Machine
- Emergency Stop Logic
- Autonomous Driving Strategy

Future improvements include:

- Sensor filtering
- Moving average smoothing
- Multi-sensor fusion with the IMU and vision system
- Safety threshold tuning

---

# Engineering Conclusion

The HC-SR04 successfully completed functional validation and was selected as the primary distance sensor for the current development stage.

Its reliability, simple interface, and stable measurements make it suitable for integration into the autonomous navigation system.
