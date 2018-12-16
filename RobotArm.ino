#include <VarSpeedServo.h>

VarSpeedServo clawPivotServo;  // create servo object to control a servo
VarSpeedServo armPivotServo;
VarSpeedServo clawServo;
VarSpeedServo baseServo;
const int servoSpeed = 100; // the servo speed
const int servoBuffer = 3; // only move the servo if the change is more than the buffer

int clawPivotPotPin = A0;  // analog pin used to connect the potentiometer for the claw pivot
int clawPivotVal;    // variable to read the value from the analog pin

int armPivotPotPin = A1;
int armPivotVal;

int clawPotPin = A2;
int clawVal;

int basePotPin = A3;
int baseVal;

int previousBaseVal = 0; // used to compare change in servo values
int previousClawVal = 0;
int previousArmPivotVal = 0;
int previousClawPivotVal = 0;


void setup() {
  clawServo.attach(10); // the claw needs to remain attached, otherwise it will loose its grip
}

void loop() {
  clawPivot();
  armPivot();
  claw();
  base();
}

void clawPivot() {
  clawPivotVal = smoothPot(clawPivotPotPin);           // reads the value of the potentiometer (value between 0 and 1023)
  clawPivotVal = map(clawPivotVal, 1023, 0, 0, 180);    // scale it to use it with the servo (value between 0 and 180)
  // only change the servo if there is significant potentiometer movement
  if (clawPivotVal + servoBuffer > previousClawPivotVal || clawPivotVal - servoBuffer < previousClawPivotVal) {
    clawPivotServo.attach(12);
    clawPivotServo.write(clawPivotVal, servoSpeed, true); // sets the servo position according to the scaled value
    clawPivotServo.detach(); // detach to avoid jitter and save power
  }
  previousClawPivotVal = clawPivotVal;
}

void armPivot() {
  armPivotVal = smoothPot(armPivotPotPin);
  armPivotVal = map(armPivotVal, 0, 1023, 0, 180);
  if (armPivotVal + servoBuffer > previousArmPivotVal || armPivotVal - servoBuffer < previousArmPivotVal) {
    armPivotServo.attach(11);
    armPivotServo.write(armPivotVal, servoSpeed, true);
    armPivotServo.detach();
  }
  previousArmPivotVal = armPivotVal;
}

void claw() {
  clawVal = smoothPot(clawPotPin);
  clawVal = map(clawVal, 1023, 0, 0, 180);
  if (clawVal + servoBuffer > previousClawVal || clawVal - servoBuffer < previousClawVal) {
    clawServo.write(clawVal, servoSpeed, true);
  }
  previousClawVal = clawVal;
}

void base() {
  baseVal = smoothPot(basePotPin);
  baseVal = map(baseVal, 0, 1023, 0, 180);
  if (baseVal + servoBuffer > previousBaseVal || baseVal - servoBuffer < previousBaseVal) {
    baseServo.attach(9);
    baseServo.write(baseVal, servoSpeed, true);
    baseServo.detach();
  }
  previousBaseVal = baseVal;
}

int smoothPot(int potPin) {
  int i;
  int potVal = 0;

  for (i = 0; i < 10; i++) {
    potVal = potVal + analogRead(potPin);    // sensor on analog pin
  }

  potVal = potVal / 10;    // average value
  return potVal;
}
