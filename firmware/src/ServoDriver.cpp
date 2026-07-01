#include "ServoDriver.h"

static Servo servoArray[SERVO_COUNT];
static uint8_t currentAngles[SERVO_COUNT];
static uint8_t targetAngles[SERVO_COUNT];

static const uint8_t servoPins[SERVO_COUNT] = {
  SERVO_LEFT_LEG_PIN,
  SERVO_RIGHT_LEG_PIN,
  SERVO_LEFT_FOOT_PIN,
  SERVO_RIGHT_FOOT_PIN
};

static uint8_t clampServoAngle(int angle) {
  if (angle < SERVO_MIN_ANGLE) return SERVO_MIN_ANGLE;
  if (angle > SERVO_MAX_ANGLE) return SERVO_MAX_ANGLE;
  return static_cast<uint8_t>(angle);
}

void servoDriverInit() {
  Serial.println("[ServoDriver] Initialisation des servos...");

  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    servoArray[i].setPeriodHertz(50);
    servoArray[i].attach(servoPins[i], 500, 2500);
    currentAngles[i] = SERVO_CENTER;
    targetAngles[i] = SERVO_CENTER;
    servoArray[i].write(SERVO_CENTER);

    Serial.print("[ServoDriver] Servo ");
    Serial.print(i);
    Serial.print(" attache au pin ");
    Serial.println(servoPins[i]);
  }

  delay(500);
  Serial.println("[ServoDriver] Initialisation complete");
}

void servoDriverUpdate() {
  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    if (currentAngles[i] < targetAngles[i]) {
      uint8_t delta = targetAngles[i] - currentAngles[i];
      currentAngles[i] += min<uint8_t>(SERVO_STEP_DEGREES, delta);
      servoArray[i].write(currentAngles[i]);
    } else if (currentAngles[i] > targetAngles[i]) {
      uint8_t delta = currentAngles[i] - targetAngles[i];
      currentAngles[i] -= min<uint8_t>(SERVO_STEP_DEGREES, delta);
      servoArray[i].write(currentAngles[i]);
    }
  }
}

void servoDriverSetAngle(unsigned int servoId, int angle) {
  servoSetAngle(static_cast<uint8_t>(servoId), clampServoAngle(angle));
}

int servoDriverGetAngle(unsigned int servoId) {
  if (servoId >= SERVO_COUNT) {
    return -1;
  }
  return servoGetAngle(static_cast<uint8_t>(servoId));
}

void servoSetAngle(uint8_t servoIndex, uint8_t angle) {
  if (servoIndex >= SERVO_COUNT) {
    Serial.println("[ServoDriver] Erreur: index servo invalide");
    return;
  }

  targetAngles[servoIndex] = clampServoAngle(angle);
}

void servoSetAngleImmediate(uint8_t servoIndex, uint8_t angle) {
  if (servoIndex >= SERVO_COUNT) {
    Serial.println("[ServoDriver] Erreur: index servo invalide");
    return;
  }

  uint8_t clampedAngle = clampServoAngle(angle);
  currentAngles[servoIndex] = clampedAngle;
  targetAngles[servoIndex] = clampedAngle;
  servoArray[servoIndex].write(clampedAngle);
}

uint8_t servoGetAngle(uint8_t servoIndex) {
  if (servoIndex >= SERVO_COUNT) {
    return 0;
  }
  return currentAngles[servoIndex];
}

void servoSetNeutralPosition() {
  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    servoSetAngle(i, SERVO_CENTER);
  }
}

void servoDisableAll() {
  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    servoArray[i].detach();
  }
  Serial.println("[ServoDriver] Tous les servos sont desactives");
}

void servoEnableAll() {
  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    if (!servoArray[i].attached()) {
      servoArray[i].attach(servoPins[i], 500, 2500);
      servoArray[i].write(currentAngles[i]);
    }
  }
  Serial.println("[ServoDriver] Tous les servos sont actives");
}
