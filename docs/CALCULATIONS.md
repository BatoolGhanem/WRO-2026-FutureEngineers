
# Engineering Calculations

## Purpose

This document contains the engineering calculations used during the design and development of our WRO Future Engineers robot.

Instead of selecting components by trial and error, engineering calculations were performed to justify the mechanical, electrical, and power system design decisions.

The calculations in this document will be updated whenever new measurements or experimental results become available.

---

# 1. Robot Dimensions

The dimensions of the robot were measured after the mechanical assembly.

| Parameter | Value |
|-----------|-------|
| Length | 248 mm |
| Width | 146 mm |
| Height | 70 mm |

---

# 2. Robot Weight

Current robot weight:

**599 g**

Current measurement does not include:

- Battery
- Camera

The final weight will be updated after the electrical system is completed.

---

# 3. Wheel Dimensions

Wheel Diameter:

**65 mm**

---

# 4. Wheel Circumference

The wheel circumference was calculated using the following equation:

Wheel Circumference = π × Diameter

= 3.1416 × 65

= **204.2 mm**

This value will later be used to estimate the theoretical vehicle speed.

---

# 5. Gear Ratio

The gearbox was completely disassembled in order to calculate the reduction ratio manually.

Each gear stage was counted individually because the manufacturer did not provide the gearbox ratio.

| Stage | Calculation | Ratio |
|--------|-------------|------:|
| Stage 1 | 22 / 12 | 1.83 |
| Stage 2 | 20 / 12 | 1.67 |
| Stage 3 | 22 / 10 | 2.20 |
| Stage 4 | 23 / 10 | 2.30 |

Total Gear Ratio

= 1.83 × 1.67 × 2.20 × 2.30

= **15.44 : 1**

> Note:
>
> The gearbox was manually disassembled and each gear tooth was counted to determine the total gear reduction ratio. The final value will be verified experimentally during future testing.
