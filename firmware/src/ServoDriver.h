#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include <ESP32Servo.h>
#include "Pinout.h"
#include "Constants.h"

// ======== SERVO DEFINITIONS ========
#define SERVO_COUNT 4

enum ServoIndex {
  SERVO_LEFT_LEG = 0,
  SERVO_RIGHT_LEG = 1,
  SERVO_LEFT_FOOT = 2,
  SERVO_RIGHT_FOOT = 3
};

// ======== FUNCTIONS ========

/**
 * Initialise tous les servos
 * - Configure les pins PWM
 * - Place les servos à la position neutre
 */
void servoDriverInit();

/**
 * Met à jour les servos avec les nouvelles positions
 * À appeler en boucle pour appliquer les changements
 */
void servoDriverUpdate();

/**
 * Définie la position (angle) d'un servo spécifique
 * @param servoIndex: Index du servo (SERVO_LEFT_LEG, etc.)
 * @param angle: Angle en degrés (0-180)
 */
void servoSetAngle(uint8_t servoIndex, uint8_t angle);

/**
 * Retourne la position actuelle d'un servo
 * @param servoIndex: Index du servo
 * @return angle actuel en degrés
 */
uint8_t servoGetAngle(uint8_t servoIndex);

/**
 * Place tous les servos à la position neutre
 */
void servoSetNeutralPosition();

/**
 * Désactive tous les servos (relâche la tension)
 */
void servoDisableAll();

/**
 * Active tous les servos
 */
void servoEnableAll();

#endif
