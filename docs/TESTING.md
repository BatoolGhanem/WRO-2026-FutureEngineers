# Testing

## Testing Objectives

The objective of the testing process was to verify that all hardware and software components operated reliably under real competition conditions. Each subsystem was tested independently before being integrated into the complete robot. The testing process focused on accuracy, repeatability, stability, and overall navigation performance.

---

## Test Environment

The robot was tested on a practice track designed to replicate the WRO Future Engineers field. Multiple test sessions were performed under different lighting conditions and with repeated laps to evaluate consistency.

The following aspects were evaluated:

- Straight-line driving
- Steering accuracy
- Wall following
- Corner detection
- Obstacle detection
- Color marker recognition
- Vision system reliability
- Full autonomous navigation

---

## Hardware Testing

Each hardware component was verified individually before integration.

| Component | Test Performed | Result |
|-----------|----------------|--------|
| ESP32 DevKit V1 | GPIO and communication verification | Passed |
| Raspberry Pi 3 Model B | Vision processing and UART communication | Passed |
| Raspberry Pi Camera | Image acquisition and object detection | Passed |
| L298N Motor Driver | Motor direction and PWM control | Passed |
| MG996R Servo | Steering angle calibration | Passed |
| VL53L1X Left | Distance measurement accuracy | Passed |
| VL53L1X Right | Distance measurement accuracy | Passed |
| MPU6050 | Heading and turn angle measurement | Passed |
| TCS34725 | Color marker detection | Passed |
| XL4015 Buck Converter | Stable 5.06 V output | Passed |

---

## Sensor Testing

The distance sensors were tested at multiple wall distances to verify measurement stability and repeatability.

The MPU6050 was tested during repeated left and right turns to validate heading estimation.

The TCS34725 color sensor was tested under different lighting conditions to ensure reliable color classification.

The Raspberry Pi Camera was tested with different obstacle positions and distances to verify consistent object detection.

---

## Motion Testing

Motor speed and steering response were tested independently before autonomous navigation.

The following characteristics were evaluated:

- Smooth acceleration
- Stable steering
- Straight-line tracking
- Turning precision
- Recovery after turns

PWM values and steering calibration were adjusted until repeatable motion was achieved.

---

## Navigation Testing

The complete navigation algorithm was evaluated by performing repeated autonomous laps.

The following behaviors were verified:

- Wall following
- Predictive corner detection
- Obstacle avoidance
- Correct steering direction
- Heading correction using the IMU
- Recovery after obstacle avoidance
- Stable lane positioning

Each software update was validated through repeated runs before being incorporated into the final version.

---

## Vision Testing

The Raspberry Pi vision system was tested independently from the ESP32 controller.

Testing focused on:

- Obstacle detection
- Detection stability
- Communication latency
- Reliable UART data transfer
- Correct synchronization with the ESP32

The communication protocol was verified to ensure accurate command transmission during autonomous driving.

---

## Reliability Testing

Long-duration testing sessions were conducted to evaluate system reliability.

The robot completed multiple consecutive laps while monitoring:

- Sensor stability
- Communication reliability
- Steering consistency
- Power stability
- Navigation repeatability

Special attention was given to maintaining consistent performance throughout the entire run.

---

## Performance Improvements

Testing results were used to refine both hardware and software.

The main improvements included:

- Steering calibration optimization
- More stable wall-following behavior
- Improved corner detection
- Better obstacle avoidance timing
- Reduced sensor noise
- Improved heading correction using the IMU
- Increased navigation consistency

---

## Final Validation

The final version of the robot was validated after completing successful autonomous runs with all subsystems operating simultaneously.

The validation confirmed:

- Reliable hardware operation
- Stable communication between controllers
- Accurate sensor measurements
- Consistent autonomous navigation
- Robust obstacle detection and avoidance
- Repeatable overall performance suitable for competition conditions
