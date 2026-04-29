# Robotic Arm — INFO-I341

A 5-servo robotic arm controlled by a joystick, built for **INFO-I341: Human-Computer Interaction** at Indiana University Bloomington.

## Overview

The arm has five servo motors: base rotation, a dual-servo shoulder (front and back moving antagonistically), an elbow clamped to a 45° range, and a toggle gripper. One joystick controls the X/Y axes while the joystick button toggles the gripper open and closed.

## Hardware

| Component | Model / Notes |
|-----------|--------------|
| Microcontroller | Arduino Uno |
| Servos | 5× standard hobby servo (e.g. SG90 or MG996R) |
| Joystick | 1× KY-023 analog joystick module |
| Power | External 5 V supply recommended for all 5 servos |

## Pin Configuration

### Servos

| Servo | Pin | Range | Start |
|-------|-----|-------|-------|
| Base (rotation) | 9 | 0°–180° | 90° |
| Shoulder Front | 10 | 0°–180° | 90° |
| Shoulder Back | 6 | 0°–180° (inverted) | 90° |
| Elbow | 5 | 45°–90° | 45° |
| Gripper | 11 | 10°–73° | 10° (open) |

### Joystick

| Input | Pin | Action |
|-------|-----|--------|
| X-axis | A0 | Base rotation |
| Y-axis | A1 | Shoulder + coordinated elbow |
| Button | 2 | Toggle gripper open/close |

> **Note:** Shoulder Front and Shoulder Back move antagonistically — one is always the inverse of the other (`180 - angle`), mimicking a bicep/tricep pair. The elbow tracks the shoulder Y-axis at half speed, clamped to a 45° window.

## Wiring Diagram

```
Arduino Uno
├── Pin  2  →  Joystick button (INPUT_PULLUP, GND on press)
├── Pin  5  →  Elbow servo signal
├── Pin  6  →  Shoulder Back servo signal
├── Pin  9  →  Base servo signal
├── Pin 10  →  Shoulder Front servo signal
├── Pin 11  →  Gripper servo signal
├── A0      →  Joystick VRx
├── A1      →  Joystick VRy
├── 5V      →  Joystick VCC
└── GND     →  Joystick GND, all servo GND
```

> **Power tip:** Connect servo power wires to an external 5 V supply (shared GND with the Arduino) rather than the Arduino's onboard 5 V pin — five servos under load can exceed the regulator's current limit.

## Installation

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x or later
- `Servo` library (bundled with the Arduino IDE — no separate install needed)

### Steps

1. Clone this repository:
   ```bash
   git clone https://github.com/vpandita-maker/robotic-arm.git
   ```
2. Open **Arduino IDE** and go to **File → Open**.
3. Navigate to the cloned folder and open `robotic_arm_i341/robotic_arm_i341.ino`.
4. Connect the Arduino Uno via USB.
5. Select **Tools → Board → Arduino AVR Boards → Arduino Uno**.
6. Select the correct port under **Tools → Port**.
7. Click **Upload** (→ arrow button).
8. Open **Tools → Serial Monitor** at **9600 baud** to confirm `5-Servo Arm Ready`.

## Usage

| Action | Effect |
|--------|--------|
| Joystick left/right | Rotate base |
| Joystick up/down | Extend/retract shoulder + elbow |
| Press joystick button | Toggle gripper open/closed |

## Tuning

Two constants at the top of the sketch can be adjusted after physical testing:

```cpp
const int GRIPPER_OPEN   = 10;  // degrees — fully open
const int GRIPPER_CLOSED = 73;  // degrees — fully closed
```

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
