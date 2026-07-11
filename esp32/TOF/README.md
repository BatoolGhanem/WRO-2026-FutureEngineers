# TOF400C-VL53L1X Distance Sensor Validation

## Objective

The objective of this test is to validate the TOF400C-VL53L1X Time-of-Flight distance sensor before integrating it into the WRO Future Engineers autonomous vehicle.

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
|-----------------|--------|
| VIN | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |
| SHUT | GPIO3 |
| INT | GPIO2 |

Communication protocol:

- I2C

---

# Test 1: I2C Scanner

## Purpose

The first step was to verify that the ESP32 could detect the sensor through the I2C bus before attempting distance measurements.

## Result

The sensor was successfully detected at the default I2C address:

```text
0x29
```

Example output:

```text
I2C device found at address 0x29
```

This confirmed that:

- Power connections were correct.
- SDA and SCL wiring were correct.
- The ESP32 could communicate with the sensor through the I2C bus.

---

# Challenge Encountered

Although the sensor was correctly detected by the I2C scanner, it could not be initialized using the first software libraries that were tested.

During development, several attempts produced errors such as:

```text
VL53L1X library initialization failed
```

and

```text
i2cRead returned Error 263
```

This indicated that the hardware was functioning correctly, but the selected software libraries were not fully compatible with the TOF400C-VL53L1X module.

---

# Engineering Investigation

To identify the cause of the problem, several debugging steps were performed:

- Verified the wiring multiple times.
- Tested the sensor using an I2C Scanner.
- Confirmed that the device always appeared at address **0x29**.
- Tested different VL53L1X Arduino libraries.
- Compared initialization methods.
- Reviewed the sensor documentation and pin configuration.

The investigation showed that the problem was software-related rather than hardware-related.

---

# Solution

The issue was resolved by switching to the **Adafruit VL53L1X** library and using the correct initialization procedure together with the SHUT and INT pins.

After updating the software implementation:

- The sensor initialized successfully.
- Continuous ranging started correctly.
- Stable distance measurements were obtained.

---

# Validation Result

The ESP32 successfully:

- Initialized the sensor.
- Started continuous ranging.
- Read distance measurements in millimeters.
- Produced stable and repeatable readings.

Example output:

```text
Adafruit VL53L1X sensor demo

VL53L1X sensor OK!

Sensor ID: 0xEACC

Ranging started

Distance: 245 mm
Distance: 248 mm
Distance: 251 mm
```

---

# Engineering Notes

This debugging process demonstrated the importance of validating both hardware and software compatibility.

Although the sensor responded correctly on the I2C bus, additional investigation was required before reliable distance measurements could be achieved.

Documenting unsuccessful attempts helped identify the correct software solution and improved the reproducibility of the project.

---

# Future Integration

The validated sensor will later be integrated with:

- Sensor Manager
- Obstacle Avoidance
- Wall Detection
- Navigation State Machine
- Safety System
- Emergency Stop Logic

The final robot will use **two TOF400C-VL53L1X sensors**.

Each sensor will be initialized independently using the SHUT pins before assigning separate I2C addresses for simultaneous operation.

---

# Reproducibility Notes

To reproduce this validation:

1. Connect the sensor according to the wiring table.
2. Upload the validation program using the Adafruit VL53L1X library.
3. Verify successful initialization.
4. Confirm continuous distance measurements.
5. Repeat the test using different object distances.

---

# Engineering Conclusion

The TOF400C-VL53L1X sensor was successfully validated after resolving a software compatibility issue during development.

The final implementation provides reliable initialization, stable distance measurements, and a suitable foundation for integrating dual TOF sensors into the final WRO Future Engineers autonomous vehicle.
