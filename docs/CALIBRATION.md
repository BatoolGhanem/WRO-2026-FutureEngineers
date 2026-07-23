# Calibration

## Overview

Before each testing session and competition run, the robot is calibrated to ensure consistent and repeatable performance. Calibration minimizes the influence of environmental changes such as lighting conditions, sensor drift, and battery voltage variations.

The calibration procedure is simple and can be completed within a few minutes.

---

## Steering Calibration

The MG996R steering servo is calibrated so that the front wheels are perfectly aligned when the steering command is centered.

The steering limits are configured as follows:

| Steering Position | Value |
|-------------------|------:|
| Left Limit | 10 |
| Center | 70 |
| Right Limit | 150 |

These values were determined experimentally to provide accurate steering while preventing mechanical overtravel.

---

## Distance Sensor Calibration

The two VL53L1X Time-of-Flight sensors are initialized separately using their XSHUT pins.

During startup:

1. The left sensor is enabled and assigned its I²C address.
2. The right sensor is enabled and assigned a different I²C address.
3. Both sensors begin continuous distance measurements.

Before navigation starts, the robot records the initial wall distances. These reference measurements are then used by the wall-following algorithm instead of relying on fixed distance values, allowing the robot to adapt to different starting positions.

---

## IMU Calibration

The MPU6050 is initialized while the robot is stationary.

At startup:

- The initial heading is stored.
- Gyroscope readings are stabilized.
- This heading becomes the reference direction for all subsequent turns.

Using a reference heading helps reduce cumulative turning errors during autonomous navigation.

---

## Color Sensor Calibration

The TCS34725 color sensor is calibrated before operation to ensure reliable marker detection.

Calibration verifies:

- Stable RGB measurements
- Consistent color classification
- Reliable detection under the expected lighting conditions

Threshold values were adjusted experimentally using repeated measurements.

---

## Vision System Calibration

The Raspberry Pi camera is checked before each run to verify:

- Proper camera focus
- Stable image acquisition
- Correct obstacle detection
- Reliable UART communication with the ESP32

The vision system is tested using sample obstacles before autonomous operation.

---

## Power Verification

Before every run, the power system is inspected.

The following items are verified:

- Battery is sufficiently charged.
- XL4015 output remains at approximately 5.06 V.
- Servo and ESP32 receive stable power.
- All ground connections are secure.

Stable power is essential for maintaining consistent sensor readings and reliable motor control.

---

## Pre-Run Checklist

Before starting the robot, the following checks are performed:

- Steering is centered.
- Distance sensors respond correctly.
- IMU initializes successfully.
- Color sensor detects markers correctly.
- Camera detects obstacles.
- UART communication is operational.
- Battery voltage is sufficient.
- All electrical connections are secure.

Only after all checks are completed is the robot ready for autonomous operation.

---

## Final Notes

The calibration process was designed to be quick, repeatable, and reliable. By verifying every major subsystem before operation, the robot maintains stable performance and minimizes the effect of environmental variations during testing and competition.
