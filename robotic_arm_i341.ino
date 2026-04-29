#include <Servo.h>

// Servo objects
Servo base;
Servo shoulderFront;
Servo shoulderBack;
Servo elbow;
Servo gripper;

// Servo pins
#define PIN_BASE           3
#define PIN_SHOULDER_FRONT 5
#define PIN_SHOULDER_BACK  6
#define PIN_ELBOW          9
#define PIN_GRIPPER        10

// Joystick pins
#define JOY1_X  A0   // controls base rotation
#define JOY1_Y  A1   // controls shoulder front/back
#define JOY2_X  A2   // controls elbow
#define JOY2_Y  A3   // controls gripper open/close
#define JOY_BTN 2    // reset all servos to home

// Servo angle limits
#define BASE_MIN           0
#define BASE_MAX           180
#define SHOULDER_FRONT_MIN 0
#define SHOULDER_FRONT_MAX 180
#define SHOULDER_BACK_MIN  0
#define SHOULDER_BACK_MAX  180
#define ELBOW_MIN          0
#define ELBOW_MAX          180
#define GRIPPER_MIN        0
#define GRIPPER_MAX        90    // half range: fully open vs fully closed

// Home position angles
#define HOME_BASE           90
#define HOME_SHOULDER_FRONT 90
#define HOME_SHOULDER_BACK  90
#define HOME_ELBOW          90
#define HOME_GRIPPER        45

// Deadzone for joystick center drift (0-512 center)
#define DEADZONE 30
// Speed of movement per loop iteration (degrees)
#define SPEED 2

int posBase           = HOME_BASE;
int posShoulderFront  = HOME_SHOULDER_FRONT;
int posShoulderBack   = HOME_SHOULDER_BACK;
int posElbow          = HOME_ELBOW;
int posGripper        = HOME_GRIPPER;

void setup() {
  base.attach(PIN_BASE);
  shoulderFront.attach(PIN_SHOULDER_FRONT);
  shoulderBack.attach(PIN_SHOULDER_BACK);
  elbow.attach(PIN_ELBOW);
  gripper.attach(PIN_GRIPPER);

  pinMode(JOY_BTN, INPUT_PULLUP);

  goHome();
  Serial.begin(9600);
  Serial.println("Robotic Arm Ready");
}

void loop() {
  // Reset to home on button press
  if (digitalRead(JOY_BTN) == LOW) {
    goHome();
    delay(300);
    return;
  }

  int joy1X = analogRead(JOY1_X) - 512;
  int joy1Y = analogRead(JOY1_Y) - 512;
  int joy2X = analogRead(JOY2_X) - 512;
  int joy2Y = analogRead(JOY2_Y) - 512;

  if (abs(joy1X) > DEADZONE) posBase = constrain(posBase + (joy1X > 0 ? SPEED : -SPEED), BASE_MIN, BASE_MAX);
  if (abs(joy1Y) > DEADZONE) {
    int delta = joy1Y > 0 ? SPEED : -SPEED;
    posShoulderFront = constrain(posShoulderFront + delta, SHOULDER_FRONT_MIN, SHOULDER_FRONT_MAX);
    // Shoulder back moves antagonistically
    posShoulderBack  = constrain(posShoulderBack  - delta, SHOULDER_BACK_MIN,  SHOULDER_BACK_MAX);
  }
  if (abs(joy2X) > DEADZONE) posElbow   = constrain(posElbow   + (joy2X > 0 ? SPEED : -SPEED), ELBOW_MIN,   ELBOW_MAX);
  if (abs(joy2Y) > DEADZONE) posGripper = constrain(posGripper + (joy2Y > 0 ? SPEED : -SPEED), GRIPPER_MIN, GRIPPER_MAX);

  base.write(posBase);
  shoulderFront.write(posShoulderFront);
  shoulderBack.write(posShoulderBack);
  elbow.write(posElbow);
  gripper.write(posGripper);

  delay(15);
}

void goHome() {
  posBase          = HOME_BASE;
  posShoulderFront = HOME_SHOULDER_FRONT;
  posShoulderBack  = HOME_SHOULDER_BACK;
  posElbow         = HOME_ELBOW;
  posGripper       = HOME_GRIPPER;

  base.write(posBase);
  shoulderFront.write(posShoulderFront);
  shoulderBack.write(posShoulderBack);
  elbow.write(posElbow);
  gripper.write(posGripper);
}
