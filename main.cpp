#include <Arduino.h>
#include <ESP32Servo.h>
#include "dcmotor.h"

#define SERVO_PIN 37

#define JOYSTICK_X_PIN 4
#define JOYSTICK_SW_PIN 41

#define DRIVE_MOTOR 1
#define DRIVE_SPEED 100

Servo myServo;

// Joystick-Mitte wird automatisch gemessen
int joystickCenterX = 2048;

// Deadzone: Bereich in der Mitte, wo der Servo stoppt
const int DEADZONE = 300;

// Continuous-Servo Werte
// 1500 = Stopp
const int SERVO_STOP_US = 1500;

// Eine Seite
const int SERVO_LEFT_MAX_US = 500;

// Andere Seite
const int SERVO_RIGHT_MAX_US = 2500;

bool motorRunning = false;

int readSmoothedAnalog(int pin, int samples = 10) {
  long sum = 0;

  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delayMicroseconds(300);
  }

  return sum / samples;
}

void calibrateJoystickCenter() {
  long sum = 0;
  const int samples = 100;

  Serial.println("Joystick loslassen...");
  Serial.println("Kalibriere Mitte...");
  delay(1000);

  for (int i = 0; i < samples; i++) {
    sum += analogRead(JOYSTICK_X_PIN);
    delay(5);
  }

  joystickCenterX = sum / samples;

  Serial.print("Joystick Center X = ");
  Serial.println(joystickCenterX);
}

void updateServoFromJoystick() {
  int rawX = readSmoothedAnalog(JOYSTICK_X_PIN);
  rawX = constrain(rawX, 0, 4095);

  int diffX = rawX - joystickCenterX;

  int servoCmd = SERVO_STOP_US;
  String direction = "STOP";

  // Mitte = Stopp
  if (abs(diffX) <= DEADZONE) {
    servoCmd = SERVO_STOP_US;
    direction = "STOP";
  }

  // Joystick nach links
  else if (diffX < -DEADZONE) {
    servoCmd = map(
      rawX,
      joystickCenterX - DEADZONE,
      0,
      SERVO_STOP_US - 50,
      SERVO_LEFT_MAX_US
    );

    direction = "LEFT";
  }

  // Joystick nach rechts
  else if (diffX > DEADZONE) {
    servoCmd = map(
      rawX,
      joystickCenterX + DEADZONE,
      4095,
      SERVO_STOP_US + 50,
      SERVO_RIGHT_MAX_US
    );

    direction = "RIGHT";
  }

  servoCmd = constrain(servoCmd, SERVO_LEFT_MAX_US, SERVO_RIGHT_MAX_US);
  myServo.writeMicroseconds(servoCmd);

  Serial.print("X=");
  Serial.print(rawX);

  Serial.print(" Center=");
  Serial.print(joystickCenterX);

  Serial.print(" Diff=");
  Serial.print(diffX);

  Serial.print(" Direction=");
  Serial.print(direction);

  Serial.print(" ServoUS=");
  Serial.print(servoCmd);
}

void updateMotorFromJoystickButton() {
  bool buttonPressed = digitalRead(JOYSTICK_SW_PIN) == LOW;

  if (buttonPressed && !motorRunning) {
    setDCSpeed(DRIVE_MOTOR, FORWARD, DRIVE_SPEED);
    motorRunning = true;
  }

  if (!buttonPressed && motorRunning) {
    stopDCMotor(DRIVE_MOTOR);
    motorRunning = false;
  }

  Serial.print(" Motor=");
  Serial.println(motorRunning ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(JOYSTICK_SW_PIN, INPUT_PULLUP);

  analogReadResolution(12);

  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, SERVO_LEFT_MAX_US, SERVO_RIGHT_MAX_US);

  initDCMotors(DC_MOTOR_1);
  stopDCMotor(DRIVE_MOTOR);

  calibrateJoystickCenter();

  myServo.writeMicroseconds(SERVO_STOP_US);

  Serial.println("Servo und Motor Joystick Control gestartet");
  Serial.println("Links/Rechts = Servo, Knopf gedrueckt = Motor Gas");
}

void loop() {
  updateServoFromJoystick();
  updateMotorFromJoystickButton();

  delay(80);
}
