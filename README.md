# 🏎️ WRO 2026 — Future Engineers
## Team NextGen Minds | Palestine 🇵🇸

> An autonomous self-driving vehicle for WRO Future Engineers 2026,
> powered by Multi-Sensor Fusion and real-time embedded control.

---

## 📋 Table of Contents

- [Our Story](#our-story)
- [Team](#team)
- [Robot Overview](#robot-overview)
- [Hardware](#hardware)
- [Software](#software)
- [Open Challenge](#open-challenge)
- [Obstacle Challenge](#obstacle-challenge)
- [Gallery](#gallery)
- [Videos](#videos)
- [Engineering Journal](#engineering-journal)

---

## 📖 Our Story

We are **Computer Engineering students at Birzeit University, Year 2**.
When we first heard about WRO Future Engineers, we had no idea what it was.
We started by researching the competition, reading the rules, and learning
what a self-driving car actually needs.

### Phase 1: Learning & Planning (May 2026)

#### Day 1 — Discovering the Competition
- First team meeting with our coach
- Studied WRO 2026 rules together
- Learned about the two challenges: Open Challenge and Obstacle Challenge
- Researched what other teams use (Arduino, Raspberry Pi, ESP32)

#### Day 2 — Dividing Responsibilities
- **Batool:** Python & Computer Vision → Raspberry Pi
- **Sara:** C++ & Embedded Systems → ESP32
- Why this split? Batool has Python background, Sara has C++ background

#### Day 3 — Choosing Components
- Originally considered Arduino → switched to ESP32 (lighter, no breadboard needed)
- Considered custom 3D-printed chassis → bought ready-made RC Ackermann kit (saves time, better wheels)
- Chose sensors:
  - VL53L1X (ToF)
  - MPU-6050 (IMU)
  - TCS34725 (Color Sensor)
- Decided:
  - Phase 1 = Open Challenge with ESP32 only
  - Phase 2 = Add Raspberry Pi + Camera

#### Day 4 — Shopping
- Purchased all components from local electronics store
- Key decisions:
  - MPU-6050 instead of BNO085
  - Open Challenge first
  - Raspberry Pi integration later

#### Day 5 — Chassis Assembly
- Assembled RC Ackermann kit
- Disassembled gearbox to inspect gear ratio
- Measured dimensions using caliper
- Chassis weight: **499g**
- Chassis width: **147mm**

#### Day 6 — Simulation & Safety
- Started sensor simulation before wiring
- Verified ESP32 communication with sensors
- Raspberry Pi 3B+ became available
- Confirmed suitability with professors

### Current Status
✅ Hardware acquired  
✅ Chassis assembled  
✅ Software architecture planned  
🔄 Wiring and firmware development in progress

---

## 👥 Team

| Name | Role | Background |
|------|------|------------|
| Batool Ghanem | Software — Python & Computer Vision | Computer Engineering, Year 2 |
| Sara Abuarra | Hardware & Firmware — C++ & ESP32 | Computer Engineering, Year 2 |

**Coach:** Osid Ali

**Institution:** Birzeit University

**Country:** Palestine 🇵🇸

---

## 🤖 Robot Overview

A self-driving RC car built on an Ackermann chassis, controlled by
an ESP32 microcontroller for real-time sensor fusion and motor control.

A Raspberry Pi 3B+ handles higher-level processing for the Obstacle Challenge.

### Specifications

| Spec | Value |
|------|-------|
| Chassis Width | 147 mm |
| Chassis Weight | 499 g |
| Total Weight | ~688 g |
| Weight Limit | 1500 g |
| Margin | ~812 g ✅ |

---

## 🔧 Hardware

### Components List

| Component | Model | Weight | Function |
|-----------|-------|--------|----------|
| Main Processor | Raspberry Pi 3B+ | ~50g | Higher-level processing |
| Motion Controller | ESP32 DevKit V1 | 9g | Motor & sensor control |
| Steering Servo | MG996R | ~55g | Front wheel steering |
| Drive Motor | DC Gear Motor | ~30g | Vehicle propulsion |
| Motor Driver | L298N | 24g | Motor power control |
| IMU | MPU-6050 | ~2g | Rotation & heading |
| Distance Sensors | VL53L1X × 2 | ~2g each | Wall following |
| Color Sensor | TCS34725 | ~1g | Line detection |
| Step-down Converter | XL6009E1 | 11g | Voltage regulation |
| Chassis | RC Ackermann Kit | 499g | Robot frame |
| Battery | LiPo 2S | 33g | Power source |

### Wiring Diagram

```text
Battery (LiPo)
├──→ L298N ──→ DC Motor
├──→ XL6009E1 ──→ Raspberry Pi 3B+
│                   └──→ ESP32 (UART)
└──→ ESP32
     ├──→ MG996R Servo
     ├──→ MPU-6050
     ├──→ VL53L1X Left
     ├──→ VL53L1X Right
     ├──→ TCS34725
     └──→ L298N Control Pins
```

### Layer Design

```text
Layer 2 (Top)
├── Raspberry Pi 3B+
└── Camera Module

Layer 1
├── ESP32
├── MPU-6050
└── XL6009E1

Layer 0 (Base)
├── Chassis
├── L298N
├── DC Motor
├── MG996R Servo
└── Wheels
```

---

## 💻 Software

### Architecture

```text
Raspberry Pi (Python)          ESP32 (C++)
─────────────────────          ───────────

camera_vision.py      ←──→     motor_control.cpp
obstacle_detect.py    ←──→     servo_control.cpp
path_planning.py      ←──→     imu_reader.cpp

                               tof_sensors.cpp
                               color_detection.cpp
                               lap_counter.cpp
```

### Core Algorithm

```cpp
while (lapCount < 3)
{
    String color = getColor();

    if (color == "ORANGE" || color == "BLUE")
    {
        detectDirection(color);
        turn90degrees();
        lapCount += 0.25;
    }
    else
    {
        int error = tofLeft - tofRight;
        int steer = error * Kp;

        setMotor(BASE_SPEED, steer);
    }
}
```

### Dependencies

#### ESP32 Arduino Framework

```text
Wire.h
ESP32Servo
Adafruit_VL53L1X
MPU6050
Adafruit_TCS34725
```

---

## 🏁 Open Challenge

### Strategy

The robot completes three autonomous laps.

#### Straight Sections
- Dual ToF sensors maintain equal distance from both walls.

#### Corner Detection
- Color sensor detects track markers.

#### Turning
- MPU-6050 performs accurate 90° turns.

#### Lap Counting
- 4 turns = 1 lap
- 12 turns = finish

### Results

| Attempt | Time | Errors | Notes |
|----------|------|---------|-------|
| Run 1 | Pending | Pending | Pending |
| Run 2 | Pending | Pending | Pending |
| Run 3 | Pending | Pending | Pending |

---

## 🚧 Obstacle Challenge

### Status
Phase 2 — Under Development

### Planned Hardware
- Raspberry Pi 3B+
- Camera Module

### Planned Software
- Python
- OpenCV

### Features
- Red/Green pillar detection
- Obstacle avoidance
- Path planning
- Autonomous parking

---

## 📸 Gallery

### Robot Photos

| Front | Back | Right |
|--------|--------|--------|
| Pending | Pending | Pending |

| Left | Top | Bottom |
|--------|--------|--------|
| Pending | Pending | Pending |

### Team Photo

Pending

---

## 🎥 Videos

| Challenge | Link |
|------------|------------|
| Open Challenge | Coming Soon |
| Obstacle Challenge | Coming Soon |

---

## 📖 Engineering Journal

See:

```text
docs/JOURNAL.md
```

for the complete engineering process.

---

## 📁 Repository Structure

```text
WRO-2026-FutureEngineers/
├── README.md
├── docs/
│   ├── JOURNAL.md
│   ├── HARDWARE.md
│   ├── WIRING.md
│   ├── UART_PROTOCOL.md
│   └── DECISIONS.md
├── src/
│   ├── esp32/
│   │   ├── main.cpp
│   │   ├── motor_control.cpp
│   │   ├── servo_control.cpp
│   │   ├── sensors.cpp
│   │   └── config.h
│   └── raspberry_pi/
│       ├── main.py
│       └── camera.py
├── images/
│   ├── car/
│   └── team/
└── videos/
```

---

**Last Updated:** June 2026

**Competition:** WRO Future Engineers 2026

**Team:** NextGen Minds

**Country:** Palestine 🇵🇸
