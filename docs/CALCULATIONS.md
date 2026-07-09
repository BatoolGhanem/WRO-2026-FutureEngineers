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

The gearbox was completely disassembled because the manufacturer did not provide the gearbox reduction ratio.

Each gear stage was inspected manually, and the number of teeth on every gear was counted to calculate the overall reduction ratio.

| Stage | Calculation | Ratio |
|--------|-------------|------:|
| Stage 1 | 22 / 12 | 1.83 |
| Stage 2 | 20 / 12 | 1.67 |
| Stage 3 | 22 / 10 | 2.20 |
| Stage 4 | 23 / 10 | 2.30 |

Overall Gear Ratio

= 1.83 × 1.67 × 2.20 × 2.30

= **15.44 : 1**

### Gearbox Inspection

The gearbox was carefully disassembled to verify the reduction ratio manually.

Each gear was inspected and the number of teeth was counted individually. This process ensured that the calculated ratio accurately represents the mechanical transmission used in the robot.

![Gearbox Inspection](/images/assembly/gearbox_internal.jpg)

**Figure 1.** Gearbox disassembled during manual gear ratio calculation.

---

## Future Calculations

The following calculations will be added after the electrical system is completed and experimental measurements become available.

- Wheel RPM
- Estimated Vehicle Speed
- Motor Torque Estimation
- Total Power Consumption
- Battery Capacity Selection
- Expected Runtime
- Safety Factor
