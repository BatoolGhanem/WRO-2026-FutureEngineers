# TOF400C-VL53L1X Distance Sensor Investigation

## Objective

The objective of this test is to investigate the TOF400C-VL53L1X Time-of-Flight distance sensor before integrating it into the WRO Future Engineers autonomous vehicle.

The sensor is intended to support:

- Obstacle detection
- Wall distance estimation
- Safety monitoring
- Emergency stop behavior
- Navigation correction

---

# Hardware

## Sensor Model

The tested sensor module is labeled:

```text
TOF400C-VL53L1X
```

The module pins are:

```text
VIN
GND
SDA
SCL
INT
SHUT
```

---

# Wiring

For single-sensor testing:

| TOF400C-VL53L1X | ESP32 |
|------------------|-------|
| VIN | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |
| SHUT | 3.3V |
| INT | Not connected |

Communication protocol:

- I2C

---

# Test 1: I2C Scanner

## Purpose

The I2C scanner was used to confirm whether the ESP32 can detect the sensor on the I2C bus.

## Result

The scanner successfully detected the sensor at:

```text
0x29
```

Example output:

```text
I2C device found at address 0x29
```

This confirms that:

- Power wiring is correct
- SDA and SCL wiring is correct
- The ESP32 can communicate with a device on the I2C bus
- The sensor responds at its default I2C address

---

# Test 2: VL53L1X Library Initialization

## Purpose

After confirming I2C detection, the sensor was tested using a VL53L1X Arduino library.

## Result

Although the sensor appears on the I2C bus at address `0x29`, the library initialization failed.

Observed error:

```text
ERROR: VL53L1X library initialization failed.
```

Earlier tests also produced I2C read errors during library initialization.

---

# Engineering Analysis

The test results show that the ESP32 can detect the sensor electrically, but the software library cannot complete sensor initialization.

This suggests that the issue is not basic wiring, but may be related to one of the following:

- Library incompatibility with the TOF400C-VL53L1X module
- Clone module behavior differing from standard VL53L1X breakout boards
- Required initialization sequence not supported by the tested library
- Different onboard configuration on the TOF400C module
- Sensor module requiring a specific manufacturer library

---

# Decision

The TOF distance sensors were temporarily postponed to avoid delaying development of the rest of the robot.

This decision was made because other critical subsystems can continue development independently, including:

- Steering control
- Motor control
- IMU validation
- Color sensor validation
- Raspberry Pi vision system
- UART communication

The TOF sensors will be revisited after the main control and vision systems are functional.

---

# Future Debugging Plan

The following steps will be performed later:

1. Test both TOF modules individually using the I2C scanner.
2. Compare both modules to confirm they are the same hardware revision.
3. Search for a TOF400C-specific Arduino library or datasheet.
4. Test Pololu and Adafruit libraries again after confirming module details.
5. Use SHUT pins to assign separate I2C addresses for dual-sensor operation.
6. Integrate the working sensors into the Safety System.

---

# Reproducibility Notes

To reproduce this test:

1. Connect the TOF400C-VL53L1X sensor using the wiring table above.
2. Upload `i2c_scanner.cpp` to the ESP32.
3. Confirm that address `0x29` appears.
4. Upload `vl53l1x_test_attempt.cpp`.
5. Observe whether the library initializes successfully.

---

# Engineering Conclusion

The TOF400C-VL53L1X module was electrically detected by the ESP32, but library-level initialization was not successful during this development phase.

The issue is documented for future investigation instead of blocking progress on the complete WRO Future Engineers autonomous vehicle software.
