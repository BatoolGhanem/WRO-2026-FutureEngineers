# Steering Test

## Objective

Verify the operation of the steering system using the MG996R servo motor connected to the ESP32.

## Initial Calibration

The first steering angles used were:

| Direction | Angle |
|----------|------:|
| Left | 50° |
| Center | 90° |
| Right | 130° |

During testing, the steering was not properly aligned. The wheels did not point straight when the servo was set to 90°, and the left turning angle was insufficient.

## Calibration Process

Several steering angles were tested until the steering linkage was correctly aligned.

The final calibrated values were:

| Direction | Angle |
|----------|------:|
| Left | 10° |
| Center | 70° |
| Right | 130° |

## Result

✅ Steering movement verified successfully.

✅ Mechanical alignment improved after calibration.

✅ The calibrated values will be used in future software development.

---

This is the first software test of the WRO Future Engineers robot.
