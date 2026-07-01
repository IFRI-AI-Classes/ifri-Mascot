#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "Pinout.h"
#include "Constants.h"

#define SERVO_COUNT 4
#define NB_SERVOS SERVO_COUNT

enum ServoIndex {
  SERVO_LEFT_LEG = 0,
  SERVO_RIGHT_LEG = 1,
  SERVO_LEFT_FOOT = 2,
  SERVO_RIGHT_FOOT = 3
};

#define SERVO_LEFT_WHEEL    SERVO_LEFT_LEG
#define SERVO_RIGHT_WHEEL   SERVO_RIGHT_LEG
#define SERVO_LEFT_SUPPORT  SERVO_LEFT_FOOT
#define SERVO_RIGHT_SUPPORT SERVO_RIGHT_FOOT

void servoDriverInit();
void servoDriverUpdate();

void servoDriverSetAngle(unsigned int servoId, int angle);
int servoDriverGetAngle(unsigned int servoId);

void servoSetAngle(uint8_t servoIndex, uint8_t angle);
void servoSetAngleImmediate(uint8_t servoIndex, uint8_t angle);
uint8_t servoGetAngle(uint8_t servoIndex);

void servoSetNeutralPosition();
void servoDisableAll();
void servoEnableAll();

#endif
