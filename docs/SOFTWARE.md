# Software Design

## Introduction

The software architecture of our WRO Future Engineers robot was designed with reliability, modularity, and maintainability as the primary objectives.

Instead of implementing a single monolithic program, the software was divided into independent modules, each responsible for a specific task such as sensor acquisition, wall following, corner detection, steering control, motor control, and mission management.

This modular design simplifies debugging, allows each subsystem to be tested independently, and makes future improvements easier without affecting the entire project.

The robot software is distributed between two processing units:

- Raspberry Pi 3 Model B
- ESP32

Each controller performs tasks that best match its computational capabilities.

---

# Software Architecture

The robot follows a distributed architecture.

The Raspberry Pi performs computationally intensive tasks such as computer vision and navigation planning, while the ESP32 performs deterministic real-time control.

This separation reduces processor load, improves timing consistency, and increases system reliability.

```
          Camera
             │
             ▼
     Raspberry Pi
 Computer Vision
 Navigation Logic
 Mission Decisions
             │
         UART Serial
             │
             ▼
           ESP32
 Real-Time Controller
             │
 ┌───────────┼────────────┐
 │           │            │
 ▼           ▼            ▼
ToF       MPU6050     TCS34725
 │           │            │
 └──────┬────┴────────────┘
        ▼
 Steering + Motor
```

---

# Software Design Philosophy

Several design principles were followed throughout the software development process.

- Modular programming
- Independent hardware abstraction
- Non-blocking execution
- State-machine based behavior
- Continuous sensor validation
- Fault detection
- Safe recovery whenever possible

These principles make the software easier to understand, maintain, and expand.

---

# Software Organization

The ESP32 firmware is organized into multiple logical modules.

Instead of placing all variables in one file, the program separates configuration constants, runtime data, sensor processing, navigation algorithms, and hardware control.

Major software sections include:

- Build configuration
- Hardware configuration
- Steering configuration
- Motor configuration
- Distance processing
- Corner detection
- IMU processing
- Color detection
- Mission management
- Safety management
- State machine

This organization significantly improves readability compared to a single large source file.

---

# Hardware Abstraction

Each hardware device is initialized independently.

The software creates dedicated objects for:

- Left VL53L1X
- Right VL53L1X
- MPU6050
- TCS34725
- Steering Servo

Separating hardware drivers from navigation logic simplifies debugging and future hardware replacement.

---

# Runtime Data Structures

Instead of using unrelated global variables, the software stores all runtime information inside dedicated structures.

Examples include:

- Distance sensor data
- IMU data
- Color sensor data
- Steering state
- Motor state
- Corner detection state
- Mission state
- Overall robot state

This approach groups related information together and improves software maintainability.

---

# State Machine

The robot behavior is controlled using a finite state machine.

Each state represents one specific stage of the mission.

The implemented states include:

- Booting
- Waiting for Start
- Following Wall
- Predicting Corner
- Turning Corner
- Finding New Wall
- Aligning to Wall
- Searching for Finish
- Finished
- Emergency Stop

Using a state machine prevents conflicting actions and ensures that only one navigation strategy is active at any time.
# Sensor Processing

Reliable sensor processing is essential for autonomous navigation. Since all navigation decisions depend on sensor measurements, each sensor is initialized, validated, filtered, and monitored independently before its data is used by the control algorithms.

The robot continuously processes information from three different sensing systems:

- Dual VL53L1X Time-of-Flight distance sensors
- MPU6050 Inertial Measurement Unit (IMU)
- TCS34725 Color Sensor

Each sensor has its own processing pipeline to maximize robustness and minimize the influence of noise.

---

# Distance Measurement

Two VL53L1X sensors are installed on the robot to continuously measure the distance between the robot and the surrounding walls.

The left and right sensors operate independently and are assigned different I²C addresses during initialization using the XSHUT pins. This prevents address conflicts while allowing both sensors to operate simultaneously.

Each distance reading passes through several validation stages before it is accepted by the navigation algorithms.

The software performs:

- Range validation
- Sensor freshness verification
- Median filtering
- Exponential filtering
- Distance rate estimation

Invalid measurements are rejected immediately without affecting the navigation controller.

---

# Median Filtering

Raw Time-of-Flight measurements occasionally contain spikes caused by reflections or temporary measurement errors.

To eliminate these abnormal values, every new measurement is inserted into a small sliding window.

The median value of this window is selected as the representative distance instead of the newest sample.

Unlike a simple average, the median filter removes isolated spikes without introducing significant delay.

This significantly improves the stability of wall-following performance.

---

# Exponential Moving Average

After the median filter, the distance is processed by an Exponential Moving Average (EMA).

The EMA smooths small measurement fluctuations while still allowing the robot to react quickly to changes in wall position.

This creates a stable distance signal suitable for steering control.

The filter continuously updates the estimated wall distance without requiring large memory buffers.

---

# Dual Distance Processing

The software maintains two independent filtered distance signals.

The first signal is optimized for wall following.

It prioritizes stability and smooth steering.

The second signal reacts much faster and is dedicated exclusively to corner detection.

Keeping these processing paths separate allows the robot to maintain smooth motion while still detecting openings as early as possible.

---

# Distance Rate Calculation

Besides measuring the distance itself, the software continuously estimates how quickly the measured distance is changing.

The rate of change provides important information about the surrounding environment.

For example:

- Stable distance indicates that the robot is still travelling beside a wall.
- Rapidly increasing distance suggests that the wall has ended.
- Rapidly decreasing distance indicates that the robot is approaching a new obstacle.

This information is later used by the predictive corner detection algorithm.

---

# IMU Processing

The MPU6050 gyroscope provides the robot with rotational information during cornering.

Before the mission starts, the software performs a complete gyroscope calibration procedure while the robot remains stationary.

Multiple samples are collected to estimate the gyroscope bias.

The calibration process also calculates the measurement variation.

If excessive movement is detected during calibration, the calibration is rejected and the robot does not continue until valid calibration is achieved.

This prevents inaccurate heading estimation throughout the mission.

---

# Gyroscope Integration

After calibration, the gyroscope angular velocity is continuously integrated over time to estimate the robot's yaw angle.

The software also removes:

- Sensor bias
- Small measurement noise
- Unrealistic angular velocities

The resulting yaw estimate is used exclusively during corner execution.

Unlike wheel encoders, the gyroscope provides accurate rotational feedback even when wheel slip occurs.

---

# Color Sensor Processing

The downward-facing TCS34725 continuously monitors the floor color.

Instead of relying on absolute RGB values, the software normalizes the measured color channels.

This normalization greatly reduces the influence of changing ambient lighting conditions.

The normalized values are then compared against predefined color boundaries to classify the detected floor color.

The robot recognizes:

- Orange
- Blue
- Neutral floor

Unknown colors are ignored to reduce false detections.

---

# Color Stability Verification

Single color readings are never trusted immediately.

The software requires multiple consecutive detections before confirming that a valid floor color has been observed.

Likewise, several consecutive neutral readings are required before releasing the previous detection.

This confirmation mechanism prevents false triggers caused by sensor noise or temporary lighting variations.

# Wall Following Controller

Wall following is the primary navigation strategy used throughout the Open Challenge.

Rather than driving along a pre-defined path, the robot continuously estimates its position relative to the inner wall and adjusts its steering angle to maintain a constant distance.

The controller uses the filtered Time-of-Flight measurements together with the desired wall distance to calculate a steering correction.

Small measurement errors inside a predefined deadband are ignored to prevent unnecessary steering oscillations.

Additionally, the controller considers the rate of change of the measured distance, allowing the robot to anticipate deviations before they become large.

The final steering command is limited to safe mechanical angles to protect the steering system and maintain stable motion.

This approach allows the robot to follow curved walls smoothly while remaining resistant to measurement noise.

---

# Predictive Corner Detection

Instead of waiting until the wall completely disappears, the robot predicts an upcoming corner before reaching it.

The prediction algorithm continuously monitors:

- The filtered wall distance.
- The rate at which the wall distance changes.
- The difference between the current distance and a previously learned wall baseline.

During normal driving, the software continuously updates a stable wall baseline representing the average distance to the current wall.

When the measured distance begins increasing rapidly while also exceeding the expected baseline, the robot enters a predictive corner state.

This allows steering adjustments to begin before the robot reaches the actual opening.

Predictive corner detection significantly reduces turning delay and improves the robot's trajectory through the corner.

---

# Stable Wall Baseline

A single distance threshold is often unreliable because the robot may start from different positions on different competition fields.

To overcome this limitation, the software continuously learns the current wall distance while travelling along a stable section of the track.

This learned reference becomes the wall baseline.

Future distance measurements are compared against this baseline instead of relying solely on fixed thresholds.

As a result, corner detection automatically adapts to small variations in robot position and field geometry.

---

# Corner Confirmation

A predicted opening does not immediately trigger a turn.

Instead, the software waits for additional evidence before confirming that a corner has actually been reached.

The confirmation process evaluates:

- Absolute wall distance.
- Relative increase from the learned baseline.
- Consecutive valid observations.

Only when sufficient evidence is collected does the robot begin the turning procedure.

This greatly reduces false corner detections caused by temporary sensor disturbances.

---

# IMU-Based Corner Turning

Once a corner is confirmed, wall following is temporarily suspended and the robot switches to inertial navigation.

The steering servo immediately moves to the required turning angle while the gyroscope continuously measures the robot's rotation.

Unlike time-based turning, the robot does not assume that every turn requires the same duration.

Instead, the actual rotation angle is measured throughout the maneuver.

The software predicts rotational overshoot using the current angular velocity and gradually slows the robot before reaching the desired heading.

This prediction minimizes overshoot while maintaining fast corner execution.

After the target angle is reached, the robot waits until the rotational velocity becomes sufficiently small before completing the turn.

This prevents premature exit while the robot is still rotating due to inertia.

---

# Wall Recovery

Immediately after leaving a corner, the robot begins searching for the new wall.

Instead of assuming the wall has already been found, the software continuously evaluates new distance measurements until several consecutive observations confirm that a stable wall is present.

Only then does the robot return to normal wall following.

This confirmation process prevents unstable steering immediately after completing a turn.

---

# Mission Management

The entire Open Challenge mission is controlled using a finite state machine.

Each navigation stage is represented by a dedicated software state.

Typical mission progression is:

Booting

↓

Waiting for Start

↓

Following Wall

↓

Predicting Corner

↓

Turning Corner

↓

Finding New Wall

↓

Aligning to Wall

↓

Following Wall

↓

Searching for Finish

↓

Finished

Only one state can be active at any moment.

This guarantees predictable software behavior and simplifies debugging.

---

# Finish Detection

The first floor color detected at the beginning of the mission determines the driving direction.

Orange indicates clockwise navigation.

Blue indicates counter-clockwise navigation.

After the required number of corners has been completed, the robot enters a dedicated finish-search state.

The finish color must be detected multiple consecutive times before the mission is considered complete.

This confirmation process prevents accidental finishes caused by temporary color readings.

---

# Safety Mechanisms

Reliability was a major objective during software development.

Several protection mechanisms continuously monitor system health during operation.

These include:

- Sensor freshness verification.
- Distance validation.
- Gyroscope calibration verification.
- Timeout monitoring.
- Invalid measurement rejection.
- Fault code reporting.
- Emergency stop state.

If a critical fault is detected, the robot safely stops and records the corresponding fault condition instead of continuing with unreliable data.

This improves robustness during testing and simplifies troubleshooting.
# Program Flow

The software executes as a continuous real-time control loop.

During each iteration, the robot updates all sensor measurements, validates the received data, updates the navigation state machine, calculates the required steering and motor commands, and finally applies the control outputs.

The execution sequence can be summarized as follows:

1. Initialize all hardware devices.
2. Configure sensors and communication interfaces.
3. Calibrate the gyroscope.
4. Wait for the start of the mission.
5. Determine the driving direction from the floor color.
6. Identify the inner wall to follow.
7. Continuously perform wall following.
8. Predict upcoming corners.
9. Confirm the opening.
10. Execute the turn using the gyroscope.
11. Recover the new wall.
12. Continue wall following.
13. Repeat until all required corners are completed.
14. Search for the finish marker.
15. Stop the robot safely.

This cyclic execution ensures deterministic behavior and allows the robot to react immediately to changes in the environment.

---

# Software Reliability

Reliability was considered throughout the software development process.

Instead of assuming that all sensors always operate correctly, the program continuously verifies that valid and recent data is available before making navigation decisions.

Several independent validation mechanisms are implemented, including:

- Distance validity checking.
- Sensor freshness monitoring.
- Consecutive confirmation logic.
- Runtime fault detection.
- Timeout protection.
- Controlled emergency stop.

These mechanisms prevent the robot from reacting to invalid measurements and improve robustness during competition.

---

# Software Optimization

Several optimization techniques were implemented to improve execution speed and maintain deterministic timing.

Examples include:

- Independent processing modules.
- Lightweight filtering algorithms.
- Fixed-size data structures.
- Non-blocking state transitions.
- Continuous sensor polling.
- Hardware abstraction.
- Gradual actuator updates.

These techniques reduce computational overhead while maintaining fast response times.

---

# Software Testing

The software was developed incrementally.

Each subsystem was tested independently before being integrated into the complete navigation system.

The testing process included:

- Individual sensor validation.
- Wall-following tuning.
- Corner detection verification.
- IMU calibration testing.
- Turning accuracy evaluation.
- Color classification testing.
- Full mission validation.

Testing individual modules before complete integration simplified debugging and reduced software development time.

---

# Future Improvements

Although the current software successfully completes the Open Challenge, several future improvements have been identified.

Possible future developments include:

- Adaptive control parameter tuning.
- Automatic sensor calibration.
- Dynamic speed optimization based on track geometry.
- More advanced obstacle prediction algorithms.
- Improved computer vision integration.
- Automatic parameter logging for performance analysis.

The modular architecture allows these features to be added without requiring major modifications to the existing software.

---

# Conclusion

The software architecture was designed to provide reliable autonomous navigation while remaining modular, maintainable, and easy to extend.

By combining real-time sensor processing, predictive corner detection, gyroscope-based turning, robust state management, and continuous fault monitoring, the robot is able to navigate the competition field consistently under varying conditions.

The modular structure also simplifies debugging, future development, and long-term maintenance, making the software suitable for both the WRO Future Engineers competition and future research-oriented improvements.
