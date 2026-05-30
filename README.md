# 🚗 WRO 2026 — Future Engineers
### Team NextGen Minds | Palestine 🇵🇸

> An autonomous self-driving vehicle for WRO Future Engineers 2026,
> powered by Multi-Sensor Fusion and real-time embedded control.

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
| 📸 | Batool Ghanem | Software — Python & Computer Vision |
| 📸 | Sara Abuarra | Hardware & Firmware — C++ & ESP32 |

**Coach:** Osid Ali
**Institution:** Birzeit University
**Country:** Palestine 🇵🇸

---

## 🤖 Robot Overview

A self-driving RC car built on an Ackermann chassis, controlled by
an ESP32 microcontroller for real-time sensor fusion and motor control.
Navigation relies on ToF wall-following, IMU-based turn execution,
and color detection for lap counting.

### Specifications

| Spec | Value |
|------|-------|
| Chassis Width | 147mm |
| Chassis Weight | 499g |
| ESP32 Weight | 9g |
| L298N Weight | 24g |
| Battery Weight | 33g |
---

## 🔧 Hardware

### Components List

| Component | Model | Function | Why We Chose It |
|-----------|-------|----------|-----------------|
| Main Controller | ESP32 | Decision making + sensor fusion | Available, affordable, sufficient for Open Challenge |
| Steering Servo | MG996R | Front wheel steering | High torque + precision |
| Drive Motor | DC Gear Motor 25mm | Rear wheel propulsion | Compatible with chassis kit |
| Motor Driver | L298N | Motor power control | Available locally |
| IMU | MPU-6050 (GY-521) | Rotation & heading measurement | Originally planned BNO085, switched to MPU-6050 — sufficient for competition duration (~3 min per run, minimal drift) |
| Distance Sensors | VL53L1X × 2 | Left/right wall following | 1mm accuracy — better than Ultrasonic |
| Color Sensor | TCS3472 | Orange/blue line detection | Reliable & low cost |
| Step-down | XL6009E1 | Voltage regulation | Powers ESP32 from battery safely |
| Chassis | RC Ackermann Kit (RoboticX) | Robot frame | Ackermann geometry for accurate steering |
| Battery | LiPo | Power source | Stable voltage throughout the run |

### Wiring Diagram

```
Battery (LiPo)
    ├──→ L298N ──→ DC Gear Motor
    ├──→ XL6009E1 Step-down ──→ ESP32
    │                              ├──→ MG996R Servo (PWM)
    │                              ├──→ MPU-6050 (I2C)
    │                              ├──→ VL53L1X Left (I2C)
    │                              ├──→ VL53L1X Right (I2C)
    │                              └──→ TCS3472 (I2C)
    └──→ L298N signal ←── ESP32 (GPIO)
```

### Layer Design

```
Layer 2 (Top):   ToF sensors (left & right) + Color Sensor (bottom front)
Layer 1:         ESP32 + MPU-6050 + Step-down (XL6009E1)
Layer 0 (Base):  Chassis + L298N + DC Motor + MG996R Servo + Wheels
```

---

## 💻 Software

### Architecture

```
ESP32 (C++ / Arduino Framework)
────────────────────────────────
main.cpp              — Main control loop
motor_control.cpp     — L298N + DC Motor
servo_control.cpp     — MG996R steering
imu_reader.cpp        — MPU-6050 yaw reading
tof_sensors.cpp       — VL53L1X wall following
color_detection.cpp   — TCS3472 line detection
lap_counter.cpp       — Turn counting logic
```

### Communication Protocol

All sensors communicate with ESP32 via I2C bus (GPIO 21, 22).
Motor and servo receive PWM signals directly from ESP32 GPIO pins.

### Core Algorithm

```cpp
while (lapCount < 3) {
  String color = getColor();         // TCS3472 reads floor color

  if (color == "ORANGE" || color == "BLUE") {
    detectDirection(color);           // First turn determines direction
    turn90degrees();                  // MPU-6050 ensures ±2° accuracy
    lapCount += 0.25;                // 4 turns = 1 complete lap
  } else {
    int error = tofLeft - tofRight;  // Wall following error
    int steer = error * Kp;          // PID steering correction
    setMotor(BASE_SPEED, steer);
  }
}
```

### Dependencies

```
C++ (ESP32 Arduino Framework):
- Wire.h               — I2C communication
- ESP32Servo           — Servo control
- Adafruit_VL53L1X    — ToF distance sensors
- MPU6050              — by ElectronicCats
- Adafruit_TCS34725   — Color sensor
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
- Camera-based color detection for red/green pillars
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

### [2026-05-18] — Day 0: Team Meeting & Planning

**What we did:**
- Met with coach to understand WRO 2026 requirements
- Studied the rulebook together
- Planned robot architecture from scratch

**Key decisions:**
- Split responsibilities: Batool → Software, Sara → Hardware & Firmware
- Sensor plan: ToF x2 (wall following), IMU (turns), Color Sensor (lines)
- Camera needed for Obstacle Challenge — planned for Phase 2

---

### [2026-05-24] — Day 1: Shopping & Component Selection

**What we did:**
- Visited electronics store and purchased all components
- Researched and selected sensors

**Key decisions:**

1. **IMU: BNO085 → MPU-6050**
   - Originally planned BNO085 (9-DOF, near-zero drift)
   - Found MPU-6050 locally at lower cost
   - Analysis: competition runs ~3 min, MPU-6050 drift negligible
   - Decision: MPU-6050 accepted ✅

2. **No Encoder Motor**
   - IMU-based turn counting sufficient (4 turns = 1 lap)
   - Reduces cost and complexity

3. **Open Challenge First Strategy**
   - Raspberry Pi not available yet
   - ESP32 + sensors sufficient for Open Challenge
   - Staged approach reduces risk

**Components purchased:**
- ESP32, VL53L1X x2, MPU-6050, TCS3472
- RC Ackermann Chassis Kit (RoboticX)
- L298N Motor Driver, LiPo Battery
- MG996R Servo, XL6009E1 Step-down
- Breadboard + jumper wires

---

### [2026-05-25] — Day 2: Chassis Assembly

**What we did:**
- Assembled RC Ackermann chassis completely
- Disassembled gearbox to count gear teeth for gear ratio calculation
- Soldered sensor connections
- Photographed all assembly steps

**What worked:**
- Chassis assembled successfully
- Ackermann steering mechanism works smoothly

**Next session:**
- Connect ESP32 to motor and servo
- Test basic movement

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
│   └── esp32/            ← C++ (Motor & Sensor Control)
├── models/               ← 3D printed parts (STL files)
├── t-photos/             ← Team photos
├── v-photos/             ← Robot photos (6 angles)
└── video/                ← Video links
```

---

*Last updated: May 2026 | WRO Future Engineers 2026 | Palestine 🇵🇸*
