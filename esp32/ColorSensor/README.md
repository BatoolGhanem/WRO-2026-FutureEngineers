# TCS34725 Color Sensor Validation Test

## Objective

The objective of this test is to validate the functionality of the TCS34725 RGB Color Sensor before integrating it into the autonomous vehicle software.

This test verifies:

- Correct I2C communication
- Successful sensor initialization
- Stable RGB measurements
- Reliable color recognition
- Live sensor response under different colored surfaces

---

# Engineering Decision

The TCS34725 color sensor was selected because it provides accurate RGB and clear light intensity measurements using an integrated IR filter.

For the WRO Future Engineers robot, the color sensor is used to identify colored navigation markers placed on the competition field.

The sensor communicates directly with the ESP32 through the I2C bus, allowing fast and reliable color detection during autonomous operation.

---

# Hardware Configuration

## Components

- ESP32 DevKit V1
- TCS34725 Color Sensor
- Jumper Wires

---

## Wiring

| TCS34725 | ESP32 |
|-----------|--------|
| VIN | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

Communication protocol:

- I2C

---

# Software Description

The test program performs the following operations:

1. Initializes Serial communication.
2. Initializes the I2C bus.
3. Detects the TCS34725 sensor.
4. Reads:
   - Red value
   - Green value
   - Blue value
   - Clear light intensity
5. Classifies the detected surface color.
6. Displays the measurements through the Serial Monitor.

---

# Test Procedure

1. Connect the sensor according to the wiring table.
2. Upload the validation program.
3. Open the Serial Monitor at **115200 baud**.
4. Place different colored objects beneath the sensor.
5. Verify that the RGB values and detected color change correctly.

---

# Expected Output

```text
TCS34725 Color Sensor Test

TCS34725 detected successfully.

R: 125
G: 42
B: 37
Clear: 215
Detected: RED
```

Example outputs may include:

```text
Detected: RED
Detected: GREEN
Detected: BLUE
Detected: WHITE
Detected: UNKNOWN
```

---

# Test Results

The ESP32 successfully initialized the TCS34725 through the I2C bus.

The sensor continuously measured RGB values and clear light intensity.

Different colored surfaces produced distinct RGB readings, allowing successful color classification.

---

# Engineering Notes

The TCS34725 shares the same I2C bus with the MPU6050 and the VL53L1X sensors.

Using a shared I2C bus reduces wiring complexity while maintaining reliable communication with multiple sensors.

During testing, different colored objects were placed beneath the sensor to verify repeatable measurements under consistent lighting conditions.

---

# Future Integration

The color sensor will later be integrated with:

- Navigation State Machine
- Sensor Manager
- Marker Detection Logic
- Raspberry Pi Vision System
- Autonomous Driving Strategy

Future improvements include:

- Dynamic color calibration
- Ambient light compensation
- Adaptive threshold tuning
- Improved marker classification under varying lighting conditions
