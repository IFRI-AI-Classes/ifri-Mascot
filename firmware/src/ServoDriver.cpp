#include "ServoDriver.h"

// ======== SERVO OBJECTS ========
Servo servoArray[SERVO_COUNT];
uint8_t currentAngles[SERVO_COUNT];
uint8_t targetAngles[SERVO_COUNT];

// Pin mapping pour les servos
const uint8_t servoPins[SERVO_COUNT] = {
  SERVO_LEFT_LEG_PIN,
  SERVO_RIGHT_LEG_PIN,
  SERVO_LEFT_FOOT_PIN,
  SERVO_RIGHT_FOOT_PIN
};

// ======== INITIALIZATION ========

void servoDriverInit() {
  Serial.println("[ServoDriver] Initialisation des servos...");
  
  // Initialiser chaque servo
  for (int i = 0; i < SERVO_COUNT; i++) {
    servoArray[i].attach(servoPins[i]);
    currentAngles[i] = SERVO_CENTER;
    targetAngles[i] = SERVO_CENTER;
    servoArray[i].write(SERVO_CENTER);
    
    Serial.print("[ServoDriver] Servo ");
    Serial.print(i);
    Serial.print(" attaché au pin ");
    Serial.println(servoPins[i]);
  }
  
  delay(500); // Laisser les servos se stabiliser
  Serial.println("[ServoDriver] Initialisation complète!");
}

void servoDriverUpdate() {
  // Cette fonction est appelée en boucle pour appliquer les changements progressifs
  for (int i = 0; i < SERVO_COUNT; i++) {
    if (currentAngles[i] != targetAngles[i]) {
      // Mouvement progressif (vitesse contrôlée)
      if (currentAngles[i] < targetAngles[i]) {
        currentAngles[i]++;
      } else {
        currentAngles[i]--;
      }
      servoArray[i].write(currentAngles[i]);
    }
  }
}

// ======== SERVO CONTROL ========

void servoSetAngle(uint8_t servoIndex, uint8_t angle) {
  if (servoIndex >= SERVO_COUNT) {
    Serial.println("[ServoDriver] Erreur: Index servo invalide!");
    return;
  }
  
  // Limiter l'angle entre min et max
  if (angle < SERVO_MIN_ANGLE) angle = SERVO_MIN_ANGLE;
  if (angle > SERVO_MAX_ANGLE) angle = SERVO_MAX_ANGLE;
  
  targetAngles[servoIndex] = angle;
}

uint8_t servoGetAngle(uint8_t servoIndex) {
  if (servoIndex >= SERVO_COUNT) {
    return 0;
  }
  return currentAngles[servoIndex];
}

void servoSetNeutralPosition() {
  for (int i = 0; i < SERVO_COUNT; i++) {
    servoSetAngle(i, SERVO_CENTER);
  }
}

void servoDisableAll() {
  for (int i = 0; i < SERVO_COUNT; i++) {
    servoArray[i].detach();
  }
  Serial.println("[ServoDriver] Tous les servos sont désactivés");
}

void servoEnableAll() {
  for (int i = 0; i < SERVO_COUNT; i++) {
    servoArray[i].attach(servoPins[i]);
  }
  Serial.println("[ServoDriver] Tous les servos sont activés");
}
