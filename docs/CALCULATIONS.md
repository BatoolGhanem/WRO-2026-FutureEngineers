# Engineering Calculations

## Purpose

This document presents the engineering calculations and quantitative analysis used during the design and development of our WRO Future Engineers 2026 robot.

Rather than selecting components through trial and error, mathematical calculations and engineering measurements were used to justify design decisions, evaluate system performance, and estimate the robot's capabilities before practical testing.

Whenever possible, theoretical calculations were compared with experimental observations to verify the accuracy of the design.

---

# 1. Robot Dimensions

The robot dimensions were measured after the mechanical assembly was completed.

| Parameter | Value |
|-----------|-------|
| Length | 248 mm |
| Width | 146 mm |
| Height | 70 mm |

These measurements confirm that the robot complies with the dimensional constraints of the WRO Future Engineers competition.

---

# 2. Robot Weight

Measured robot weight (without battery and camera):

**599 g**

The measured value currently excludes:

- Battery
- Raspberry Pi Camera

The complete robot weight will be updated after the final competition configuration is assembled.

Robot weight directly influences acceleration, braking performance, motor load, and overall power consumption.

---

# 3. Wheel Dimensions

Drive wheel diameter:

**65 mm**

Accurate wheel measurements are essential because they influence vehicle speed estimation, turning behaviour, and encoder-free distance calculations.

---

# 4. Wheel Circumference

The wheel circumference was calculated using:

Wheel Circumference = π × Diameter

= 3.1416 × 65

= **204.2 mm**

This value represents the theoretical distance travelled during one complete wheel revolution.

---

# 5. Gear Ratio Analysis

The gearbox manufacturer did not specify the internal reduction ratio.

To determine the actual transmission ratio, the gearbox was completely disassembled and every gear was inspected manually.

The number of teeth on every gear was counted individually.

| Stage | Calculation | Ratio |
|--------|-------------|------:|
| Stage 1 | 22 / 12 | 1.83 |
| Stage 2 | 20 / 12 | 1.67 |
| Stage 3 | 22 / 10 | 2.20 |
| Stage 4 | 23 / 10 | 2.30 |

Overall Gear Ratio

= (22/12) × (20/12) × (22/10) × (23/10)

= **15.46 : 1**

### Gearbox Inspection

The gearbox was carefully disassembled to verify the reduction ratio manually.

Every gear stage was inspected before calculating the final reduction ratio.

![Gearbox Inspection](/images/assembly/gearbox_internal.jpg)

**Figure 1.** Manual gearbox inspection during reduction ratio calculation.

---

# 6. Theoretical Wheel Speed

The theoretical wheel rotational speed can be estimated using:

Wheel RPM = Motor RPM / Gear Ratio

Using the measured gearbox ratio:

Wheel RPM = Motor RPM / 15.46

The final numerical value depends on the motor speed at the actual operating voltage.

---

# 7. Theoretical Vehicle Speed

The theoretical vehicle speed is estimated using the wheel circumference and wheel rotational speed.

Vehicle Speed = Wheel Circumference × Wheel RPM

or

Vehicle Speed (m/s)

= (204.2 × Wheel RPM) / 60000

The calculated value represents the ideal speed before considering wheel slip, drivetrain losses, battery voltage drop, and mechanical friction.

Actual performance is expected to be slightly lower.

---

# 8. Experimental Performance Measurements

Practical testing is used to validate the theoretical calculations.

The robot is tested by driving a known distance while measuring the travel time.

Vehicle Speed = Distance / Time

Multiple experimental runs are performed to evaluate repeatability and reduce measurement uncertainty.

Future measurements will include:

- Average vehicle speed
- Maximum vehicle speed
- Lap consistency
- Speed variation during turning

---

# 9. Steering Evaluation

Because the robot uses an Ackermann steering system, steering geometry plays an important role in navigation accuracy.

During testing, steering behaviour is evaluated by observing:

- Turning consistency
- Turning position
- Turning smoothness
- Repeatability over multiple laps

Practical testing is used to determine the steering parameters that produce the most reliable autonomous navigation.

---

# 10. Power System Analysis

Electrical power requirements are evaluated after integrating the complete robot.

The electrical power consumed by each subsystem is determined using:

Power = Voltage × Current

Future measurements will include:

| Component | Voltage | Current | Power |
|-----------|---------|---------|------|
| ESP32 | To be measured | To be measured | To be calculated |
| Raspberry Pi 3 Model B | To be measured | To be measured | To be calculated |
| Drive Motor | To be measured | To be measured | To be calculated |
| Steering Servo | To be measured | To be measured | To be calculated |
| Sensors | To be measured | To be measured | To be calculated |

The total system power will be calculated by summing the power consumption of all subsystems.

---

# 11. Battery Runtime Estimation

The expected operating time is estimated using:

Runtime (hours)

= Battery Capacity (Ah)

÷ Average Current (A)

The theoretical value will be compared with the actual runtime measured during continuous autonomous operation.

---

# 12. Engineering Observations

Engineering calculations were not performed only for documentation purposes.

They directly influenced several design decisions throughout the project, including:

- Selecting the drivetrain configuration.
- Understanding gearbox performance.
- Estimating theoretical vehicle speed.
- Planning battery requirements.
- Evaluating steering performance.
- Comparing theoretical predictions with practical testing.

These calculations provided quantitative support for many engineering decisions made during the development of the robot.

---

# Future Updates

This document will continue to evolve as additional measurements become available during future testing.

Planned additions include:

- Final robot weight
- Experimental vehicle speed
- Battery runtime measurements
- Current consumption measurements
- Power consumption analysis
- Steering geometry measurements
- Turning radius evaluation
- Long-term performance measurements

Continuous measurement and validation remain an essential part of the engineering development process.
