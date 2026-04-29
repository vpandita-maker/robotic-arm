# Robotic Arm — INFO-I341

A 5-servo robotic arm controlled by dual joysticks, built for **INFO-I341: Human-Computer Interaction** at Indiana University Bloomington.

## Overview

The arm consists of five servo motors providing full articulation: base rotation, a dual-servo shoulder (front and back working antagonistically), elbow bend, and a gripper. Two analog joysticks map intuitively to each axis, with a push-button to snap the arm back to its home position.

## Hardware

| Component | Model / Notes |
|-----------|--------------|
| Microcontroller | Arduino Uno |
| Servos | 5× standard hobby servo (e.g. SG90 or MG996R) |
| Joysticks | 2× KY-023 analog joystick module |
| Power | External 5 V supply recommended for all 5 servos |

## Pin Configuration

### Servos

| Servo | Pin | Range | Home |
|-------|-----|-------|------|
| Base (rotation) | 3 | 0°–180° | 90° |
| Shoulder Front | 5 | 0°–180° | 90° |
| Shoulder Back | 6 | 0°–180° | 90° |
| Elbow | 9 | 0°–180° | 90° |
| Gripper | 10 | 0°–90° | 45° |

### Joysticks

| Input | Pin | Action |
|-------|-----|--------|
| Joystick 1 X-axis | A0 | Base rotation |
| Joystick 1 Y-axis | A1 | Shoulder front/back |
| Joystick 2 X-axis | A2 | Elbow bend |
| Joystick 2 Y-axis | A3 | Gripper open/close |
| Joystick button | 2 | Reset to home position |

> **Note:** The shoulder front and back servos move antagonistically — when the front servo extends forward, the back servo retracts by the same amount, mimicking a bicep/tricep pair.

## Wiring Diagram

```
Arduino Uno
├── Pin  2  →  Joystick button (INPUT_PULLUP, GND on press)
├── Pin  3  →  Base servo signal
├── Pin  5  →  Shoulder Front servo signal
├── Pin  6  →  Shoulder Back servo signal
├── Pin  9  →  Elbow servo signal
├── Pin 10  →  Gripper servo signal
├── A0      →  Joystick 1 VRx
├── A1      →  Joystick 1 VRy
├── A2      →  Joystick 2 VRx
├── A3      →  Joystick 2 VRy
├── 5V      →  Joystick VCC × 2
└── GND     →  Joystick GND × 2, all servo GND
```

> **Power tip:** Connect servo power wires to an external 5 V supply (shared GND with the Arduino) rather than the Arduino's 5 V pin — five servos under load can easily exceed the onboard regulator's current limit.

## Installation

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x or later
- `Servo` library (bundled with the Arduino IDE — no separate install needed)

### Steps

1. Clone this repository:
   ```bash
   git clone https://github.com/<your-username>/robotic_arm_i341.git
   ```
2. Open **Arduino IDE** and go to **File → Open**.
3. Navigate to the cloned folder and open `robotic_arm_i341/robotic_arm_i341.ino`.
4. Connect the Arduino Uno via USB.
5. Select **Tools → Board → Arduino AVR Boards → Arduino Uno**.
6. Select the correct port under **Tools → Port**.
7. Click **Upload** (→ arrow button).
8. Open **Tools → Serial Monitor** at **9600 baud** to confirm `Robotic Arm Ready`.

## Usage

| Action | Effect |
|--------|--------|
| Push Joystick 1 left/right | Rotate base |
| Push Joystick 1 up/down | Extend/retract shoulder |
| Push Joystick 2 left/right | Bend elbow |
| Push Joystick 2 up/down | Open/close gripper |
| Press joystick button | Return all joints to home (90°/45°) |

## Project Structure

```
robotic_arm_i341/
├── robotic_arm_i341.ino   # Main Arduino sketch
├── README.md
└── .gitignore
```

## Course

**INFO-I341 — Human-Computer Interaction**  
Luddy School of Informatics, Computing, and Engineering  
Indiana University Bloomington
