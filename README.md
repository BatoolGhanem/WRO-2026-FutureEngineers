# 🚗 WRO 2026 — Future Engineers
### Team [Your Team Name] | Palestine 🇵🇸

> An autonomous self-driving vehicle for WRO Future Engineers 2026,
> powered by Computer Vision and Multi-Sensor Fusion.

---

## 📋 Table of Contents
- [Team](#team)
- [Robot Overview](#robot-overview)
- [Hardware](#hardware)
- [Software](#software)
- [Open Challenge](#open-challenge)
- [Obstacle Challenge](#obstacle-challenge)
- [Robot Photos](#robot-photos)
- [Videos](#videos)
- [Engineering Journal](#engineering-journal)

---

## 👥 Team

| | Name | Role |
|--|------|------|
| 📸 | [Your Name] | Software — Python & Computer Vision |
| 📸 | [Partner's Name] | Hardware & Firmware — C++ & ESP32 |

**Coach:** [Coach Name]
**Institution:** [University/School Name]
**Country:** Palestine 🇵🇸

---

## 🤖 Robot Overview

A self-driving RC car built on an Ackermann chassis, powered by
a Raspberry Pi 5 for real-time Computer Vision and an ESP32/Pico
for low-level motor control. Navigation relies on Camera + ToF
sensors + IMU fusion.

### Specifications

| Spec | Value |
|------|-------|
| Dimensions | 26cm × 16cm × 20cm |
| Weight | ~800g |
| Max Speed | ~0.8 m/s |
| Steering Accuracy | ±2° |
| Battery Life | ~20 min |
| Charge Time | ~45 min |

---

## 🔧 Hardware

### Components List

| Component | Model | Function | Why We Chose It |
|-----------|-------|----------|-----------------|
| Main Processor | Raspberry Pi 5 (4GB) | Computer Vision + Decision Making | Best processing power in price range |
| Motion Controller | RPi Pico 2 / ESP32 | PWM for motor & servo | Real-time C++ response |
| Camera | RPi Camera Module 3 Wide | Track & obstacle detection | 120° FOV — sees turns early |
| IMU | BNO085 | Rotation & heading measurement | 0.1° accuracy — perfect for turns |
| Distance Sensors | VL53L1X × 2 | Left/right wall distance | 1mm accuracy — better than Ultrasonic |
| Color Sensor | TCS34725 | Orange/blue line detection | Reliable & low cost |
| Drive Motor | JGA25-370 + Encoder | Wheel propulsion | Encoder for precise distance tracking |
| Steering Servo | MG996R | Front wheel steering | High torque + precision |
| Motor Driver | BTS7960 | Motor power control | More efficient than L298N |
| Battery | LiPo 3S 2200mAh | Power source | Stable voltage throughout the run |
| Chassis | RC Ackermann Kit | Robot frame | Ackermann geometry for accuracy |

### Wiring Diagram

```
LiPo 3S (11.1V)
    ├──→ BTS7960 ──→ JGA25-370 Motor
    ├──→ LM2596 (5V) ──→ Raspberry Pi 5
    │                        ├──→ Camera (CSI cable)
    │                        ├──→ BNO085 (I2C: GPIO 2,3)
    │                        ├──→ VL53L1X Left (I2C)
    │                        ├──→ VL53L1X Right (I2C)
    │                        ├──→ TCS34725 (I2C)
    │                        └──→ Pico/ESP32 (UART: GPIO 14,15)
    └──→ Pico/ESP32 (3.3V reg)
             ├──→ BTS7960 (PWM: GPIO 4)
             └──→ MG996R Servo (PWM: GPIO 15)
```

### Layer Design

```
Layer 3 (Top):   Camera (30° downward) + ToF sensors (left & right)
Layer 2:         Raspberry Pi 5 + BNO085 IMU + Step-down module
Layer 1:         LiPo Battery + BTS7960 + Pico/ESP32
Layer 0 (Base):  Chassis + Motor + Servo + Wheels
```

---

## 💻 Software

### Architecture

```
Pi 5 (Python)                      Pico/ESP32 (C++)
─────────────                      ────────────────
camera_vision.py    ──UART──→     motor_control.cpp
wall_detection.py                  servo_control.cpp
color_detection.py   ←─UART──     imu_reader.cpp
lap_counter.py
main.py
```

### Communication Protocol

```
Pi → Pico:   "S{speed}T{steering}\n"   e.g. "S60T-15\n"
Pico → Pi:   "Y{yaw}E{encoder}\n"      e.g. "Y87.3E1250\n"
```

### Core Algorithm

```python
while lap_count < 3:
    color = color_sensor.read()

    if color in ["ORANGE", "BLUE"]:
        detect_direction(color)   # First turn determines direction
        turn_90_degrees()         # IMU ensures ±2° accuracy
        lap_count += 0.25         # 4 turns = 1 lap
    else:
        error = tof_left - tof_right
        steering = Kp * error     # PID wall-following
        send_command(speed, steering)
```

### Dependencies

```
Python (Pi 5):          C++ (Pico/ESP32):
- picamera2             - Pico SDK / Arduino ESP32
- opencv-python         - Wire.h (I2C)
- smbus2 (I2C)          - ESP32Servo / hardware/pwm.h
- pyserial (UART)       - SparkFun BNO085
```

---

## 🏁 Open Challenge

### Strategy

The robot completes 3 full laps autonomously:

1. **Straights:** Dual ToF sensors maintain equal distance from both walls
2. **Turn Detection:** Color sensor detects orange/blue lines ~5cm ahead
3. **Turn Execution:** IMU rotates exactly 90° with ±2° precision
4. **Lap Counting:** Every 4 turns = 1 complete lap → stop at 12 turns

### Results

| Attempt | Time | Errors | Notes |
|---------|------|--------|-------|
| Run 1 | — | — | — |
| Run 2 | — | — | — |
| Run 3 | — | — | — |

*(Updated continuously)*

---

## 🚧 Obstacle Challenge

*(In development — Phase 2)*

Strategy will involve:
- OpenCV color detection for red/green pillars
- Left/right avoidance logic based on pillar color
- Parallel parking in the final zone

---

## 📸 Robot Photos

| Front | Back | Right |
|-------|------|-------|
| [photo] | [photo] | [photo] |

| Left | Top | Bottom |
|------|-----|--------|
| [photo] | [photo] | [photo] |

---

## 🎥 Videos

| Challenge | Link |
|-----------|------|
| Open Challenge | [Watch](#) |
| Obstacle Challenge | *(coming soon)* |

---

## 📖 Engineering Journal

> **This is what separates us from other teams.**
> We document everything — including failures and what we learned from them.

### [2026-05-21] — Day 1: Project Kickoff

**What we did:**
- Created the GitHub repository
- Read WRO 2026 rulebook cover to cover
- Sketched initial robot design

**Decisions made:**
- Chose RC Ackermann Chassis instead of designing from scratch
  → saves time for programming (the real differentiator)
- Starting with ESP32 until Raspberry Pi 5 arrives

**Challenges:**
- None yet

**Next steps:**
- Buy components on Saturday
- Assemble chassis on Sunday

---

*(Journal updated after every work session)*

---

## 📁 Repository Structure

```
WRO-2026-FutureEngineers/
├── README.md
├── docs/
│   ├── journal.md        ← Detailed engineering journal
│   ├── hardware.md       ← Full hardware documentation
│   └── wiring/           ← Wiring diagrams & schematics
├── src/
│   ├── pi/               ← Python (Computer Vision)
│   └── esp32/            ← C++ (Motor Control)
├── models/               ← 3D printed parts (STL files)
├── t-photos/             ← Team photos
├── v-photos/             ← Robot photos (6 angles)
└── video/                ← Video links
```

---

*Last updated: May 2026 | WRO Future Engineers 2026 | Palestine 🇵🇸*
