# Engineering Journal

## Project Timeline

This journal documents the engineering development of our WRO Future Engineers 2026 robot from the initial research stage to the final autonomous vehicle.

Instead of documenting only the final robot, this journal records the complete engineering process, including research, design decisions, hardware assembly, software development, testing, failures, improvements, and validation.

---

# Phase 1 — Research & Planning

**Period:** March 2026 – April 2026

## Objective

Before purchasing any hardware, our objective was to understand the competition requirements and design an autonomous robot that would be reliable, modular, and continuously improvable throughout the season.

## Research Questions

| Engineering Question | Why was it important? | Final Decision |
|----------------------|-----------------------|----------------|
| Which controller should control the robot? | Real-time motor and sensor control is essential. | ESP32 |
| Which computing platform should process computer vision? | Camera processing requires higher computational power. | Raspberry Pi 3 Model B |
| Which distance sensor should be used? | Accurate and reliable wall detection. | VL53L1X |
| Which steering mechanism should be used? | Stable autonomous navigation. | Ackermann Steering |
| How should the robot be designed? | Allow future upgrades without rebuilding the robot. | Modular architecture |

## Engineering Goals

Our project was not only about participating in the competition.

We wanted to challenge ourselves by building a complete autonomous robot from scratch and gain practical engineering experience beyond our university courses.

Throughout the project, we focused on learning new technologies, experimenting with different solutions, and continuously improving the robot after every test.

Although learning was our main motivation, we were equally determined to build the best possible robot and compete at the highest level in WRO Future Engineers.

## Engineering Design Meeting 01

<p align="center">
  <img src="../images/research/research_planning_01.jpg" width="80%">
</p>

**Figure 1.** Research and planning session before selecting the robot architecture.

During this stage, we studied the official WRO Future Engineers rules, evaluated different hardware platforms, and compared multiple technical solutions before making any purchasing decisions.

### Objective

Understand the competition requirements and evaluate possible hardware platforms before making engineering decisions.

### Outcome

- Defined the main engineering objectives.
- Identified the robot's functional requirements.
- Selected the initial system architecture for future development.
