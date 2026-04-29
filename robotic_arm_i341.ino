#include <Servo.h>
#include <math.h>

// Define all 5 servos
Servo servoBase;
Servo servoShoulderFront;
Servo servoShoulderBack;
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
const int DEADZONE    = 30;
const int SPEED       = 2;
const int ELBOW_SPEED = 2;   // bumped to match shoulder so elbow keeps up

// Gravity compensation — when the shoulder is horizontal the elbow
// needs extra angle to hold the top up against gravity.
// Increase GRAVITY_OFFSET if the top still droops; decrease if it overcorrects.
const int GRAVITY_OFFSET = 20;

// Gripper positions (adjust after testing)
const int GRIPPER_OPEN   = 10;
const int GRIPPER_CLOSED = 73;

// Elbow range — expanded to give gravity compensation room to work
const int ELBOW_MIN = 30;
const int ELBOW_MAX = 110;

// Servo positions
int posBase     = 90;
int posShoulder = 90;
int posElbow    = 45;

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
  servoShoulderBack.write(180 - posShoulder);
  servoElbow.write(applyGravityComp(posElbow, posShoulder));
  servoGripper.write(GRIPPER_OPEN);

  pinMode(PIN_JOY_BTN, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("5-Servo Arm Ready — gravity compensation ON");
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
    if (offsetY > 0) posShoulder = min(posShoulder + SPEED, 180);
    else             posShoulder = max(posShoulder - SPEED, 0);

    servoShoulderFront.write(posShoulder);
    servoShoulderBack.write(180 - posShoulder);

    if (offsetY > 0) posElbow = min(posElbow + ELBOW_SPEED, ELBOW_MAX);
    else             posElbow = max(posElbow - ELBOW_SPEED, ELBOW_MIN);
  }

  // Always write elbow with gravity compensation applied
  servoElbow.write(applyGravityComp(posElbow, posShoulder));

  // ===== GRIPPER TOGGLE (Button) =====
  bool currentBtnState = digitalRead(PIN_JOY_BTN);

  if (currentBtnState == LOW && lastBtnState == HIGH) {
    gripperClosed = !gripperClosed;
    servoGripper.write(gripperClosed ? GRIPPER_CLOSED : GRIPPER_OPEN);

    Serial.print(">>> Gripper: ");
    Serial.println(gripperClosed ? "CLOSED" : "OPEN");

    delay(200);
  }
  lastBtnState = currentBtnState;

  delay(15);
}

// Adds a gravity compensation offset to the elbow angle.
// cos() peaks at 1 when the shoulder is horizontal (90°) and
// falls to 0 when the arm points straight up or down — which
// matches how gravity's effect on the joint actually behaves.
int applyGravityComp(int elbowAngle, int shoulderAngle) {
  float rad     = (shoulderAngle - 90.0) * PI / 180.0;
  int   gravComp = (int)(GRAVITY_OFFSET * cos(rad));
  return constrain(elbowAngle + gravComp, ELBOW_MIN, ELBOW_MAX);
}
