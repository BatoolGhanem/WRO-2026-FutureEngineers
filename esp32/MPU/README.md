# MPU6050 Validation Test

## Objective

The purpose of this test is to validate the functionality of the MPU6050 Inertial Measurement Unit (IMU) before integrating it into the complete WRO Future Engineers autonomous vehicle software.

The objective is to verify:

- Correct I2C communication
- Stable accelerometer measurements
- Stable gyroscope measurements
- Reliable sensor initialization

---

# Engineering Rationale

The MPU6050 was selected because it provides both acceleration and angular velocity measurements in a compact and lightweight package.

Although lane following is mainly performed using computer vision on the Raspberry Pi, the IMU provides additional information about the vehicle's motion that can later be used for:

- Heading estimation
- Motion analysis
- Steering stabilization
- Future sensor fusion

---

# Hardware Configuration

## Components

- ESP32 DevKit V1
- MPU6050 IMU

## Wiring

| MPU6050 | ESP32 |
|----------|--------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

Communication protocol:

- I2C

---

# Software Description

The software performs the following steps:

1. Initialize Serial communication.
2. Initialize the I2C bus.
3. Detect the MPU6050.
4. Configure:
   - Accelerometer Range
   - Gyroscope Range
   - Digital Filter
5. Continuously read:
   - Acceleration
   - Angular velocity
   - Temperature
6. Display the measurements through Serial Monitor.

---

# Test Procedure

1. Connect the sensor according to the wiring diagram.
2. Upload the validation program.
3. Open the Serial Monitor at 115200 baud.
4. Verify successful initialization.
5. Move the sensor in different directions.
6. Confirm that all measurements change smoothly.

---

# Expected Result

Successful initialization:

```text
MPU6050 found!
```

Live measurements:

```text
Acceleration
Gyroscope
Temperature
```

The sensor should continuously update without communication errors.

---

# Validation Result

The sensor initialized successfully.

The ESP32 established stable I2C communication.

Acceleration and gyroscope measurements responded correctly to physical movement.

No communication failures were observed during testing.

---

# Engineering Notes

During development, the default ESP32 I2C pins were used:

- SDA → GPIO21
- SCL → GPIO22

Using the default hardware I2C pins simplified debugging and ensured compatibility with the remaining I2C sensors that will be integrated later in the project.

---

# Future Integration

This module will become part of the final software architecture.

The IMU data will later be combined with:

- Steering Controller
- Navigation State Machine
- Safety System
- Computer Vision decisions from the Raspberry Pi

to improve vehicle stability and navigation accuracy.
