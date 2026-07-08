# DC Motor Driver Validation Test (L298N)

## Objective

The objective of this test is to verify the correct operation of the L298N motor driver and validate the communication between the ESP32 and the motor driver before integrating the propulsion system into the autonomous vehicle.

This test confirms that the ESP32 can generate the required control signals to drive the DC motor forward, reverse, and stop safely.

---

# Engineering Decision

The L298N motor driver was selected because it provides a simple and reliable solution for controlling the DC propulsion motor during the development of the robot.

The driver supports:

- Forward rotation
- Reverse rotation
- PWM speed control
- Simple GPIO interface with the ESP32

Its straightforward interface makes software debugging and hardware validation easier during the development process.

---

# Hardware Configuration

## Components

- ESP32 DevKit V1
- L298N Motor Driver
- DC Gear Motor

---

## Wiring

| ESP32 GPIO | L298N |
|------------|--------|
| GPIO25 | IN1 |
| GPIO26 | IN2 |
| GPIO27 | ENA |
| GND | GND |

---

## Motor Connection

| L298N | DC Motor |
|--------|-----------|
| OUT1 | Motor Terminal A |
| OUT2 | Motor Terminal B |

---

## Power

The ESP32 is powered through USB during software testing.

The DC motor requires an external power source and should not be powered directly from the ESP32 USB connection.

---

# Software Description

The test program performs the following sequence:

1. Initialize the GPIO pins.
2. Rotate the motor forward.
3. Stop the motor.
4. Rotate the motor in reverse.
5. Stop the motor.
6. Repeat the cycle continuously.

Serial messages are transmitted to assist with debugging and software verification.

---

# Test Procedure

1. Connect the ESP32 and L298N according to the wiring table.
2. Upload the motor test program.
3. Open the Serial Monitor at **115200 baud**.
4. Verify that the control sequence is executed correctly.
5. If an external power source is connected, verify the motor rotates in both directions.

---

# Expected Output

```text
L298N Motor Test Started

Forward
Stop
Reverse
Stop
```

---

# Test Results

The ESP32 successfully generated all required control signals.

The motor driver correctly received the commands for:

- Forward rotation
- Reverse rotation
- Stop

No communication or GPIO errors were observed during testing.

---

# Engineering Notes

The motor control software was developed as an independent module to simplify debugging and future maintenance.

Separating the propulsion subsystem from the remaining software architecture allows each hardware component to be validated individually before full system integration.

This modular design follows the engineering workflow used throughout the project.

---

# Future Integration

The motor driver module will later be integrated with:

- Steering Controller
- Navigation State Machine
- Obstacle Avoidance
- Vision-Based Navigation
- Safety System

Future improvements include:

- Closed-loop speed control using encoder feedback
- PID speed regulation
- Speed adjustment based on navigation decisions
- Emergency stop handling
