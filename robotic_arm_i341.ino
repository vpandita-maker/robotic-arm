#include <Servo.h>

// Define all 5 servos
Servo servoBase;
Servo servoShoulderFront;  // Front tilt servo (top circled)
Servo servoShoulderBack;   // Back tilt servo (bottom circled)
Servo servoElbow;
Servo servoGripper;

// Pin definitions
const int PIN_JOY_X     = A0;
const int PIN_JOY_Y     = A1;
const int PIN_JOY_BTN   = 2;

const int PIN_SERVO_BASE           = 9;
const int PIN_SERVO_SHOULDER_FRONT = 10;
const int PIN_SERVO_SHOULDER_BACK  = 6;
const int PIN_SERVO_ELBOW          = 5;
const int PIN_SERVO_GRIPPER        = 11;

// Control parameters
const int DEADZONE = 30;
const int SPEED    = 2;
const int ELBOW_SPEED = 1;  // Separate speed control for elbow

// Gripper positions (adjust these after testing)
const int GRIPPER_OPEN   = 10;   // Adjust based on your gripper
const int GRIPPER_CLOSED = 73;   // Adjust based on your gripper

// Elbow range — clamped to 45 degrees (from 45 to 90)
const int ELBOW_MIN = 45;   // lowest position
const int ELBOW_MAX = 90;   // highest position (45° range total)

// Servo positions
int posBase     = 90;
int posShoulder = 90;
int posElbow    = 45;   // starts at the minimum

// Gripper state
bool gripperClosed = false;
bool lastBtnState  = HIGH;

void setup() {
  servoBase.attach(PIN_SERVO_BASE);
  servoShoulderFront.attach(PIN_SERVO_SHOULDER_FRONT);
  servoShoulderBack.attach(PIN_SERVO_SHOULDER_BACK);
  servoElbow.attach(PIN_SERVO_ELBOW);
  servoGripper.attach(PIN_SERVO_GRIPPER);

  servoBase.write(posBase);
  servoShoulderFront.write(posShoulder);
  servoShoulderBack.write(180 - posShoulder);  // Inverted
  servoElbow.write(posElbow);
  servoGripper.write(GRIPPER_OPEN);

  pinMode(PIN_JOY_BTN, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("5-Servo Arm Ready - Elbow clamped to 45°");
}

void loop() {
  // ===== BASE ROTATION (X-axis) =====
  int rawX    = analogRead(PIN_JOY_X);
  int offsetX = rawX - 512;

  if (abs(offsetX) > DEADZONE) {
    if (offsetX > 0) posBase = min(posBase + SPEED, 180);
    else             posBase = max(posBase - SPEED, 0);
    servoBase.write(posBase);
  }

  // ===== DUAL SHOULDER + ELBOW (Y-axis) =====
  int rawY    = analogRead(PIN_JOY_Y);
  int offsetY = rawY - 512;

  if (abs(offsetY) > DEADZONE) {
    // Both shoulder servos move together (one inverted)
    if (offsetY > 0) posShoulder = min(posShoulder + SPEED, 180);
    else             posShoulder = max(posShoulder - SPEED, 0);

    servoShoulderFront.write(posShoulder);
    servoShoulderBack.write(180 - posShoulder);  // Inverted for mechanical sync

    // Elbow coordinated movement — clamped to ELBOW_MIN / ELBOW_MAX
    if (offsetY > 0) posElbow = min(posElbow + ELBOW_SPEED, ELBOW_MAX);
    else             posElbow = max(posElbow - ELBOW_SPEED, ELBOW_MIN);
    servoElbow.write(posElbow);
  }

  // ===== GRIPPER TOGGLE (Button) =====
  bool currentBtnState = digitalRead(PIN_JOY_BTN);

  if (currentBtnState == LOW && lastBtnState == HIGH) {
    gripperClosed = !gripperClosed;
    servoGripper.write(gripperClosed ? GRIPPER_CLOSED : GRIPPER_OPEN);

    Serial.print(">>> Gripper: ");
    Serial.println(gripperClosed ? "CLOSED" : "OPEN");

    delay(200);  // Debounce delay
  }
  lastBtnState = currentBtnState;

  delay(15);
}
