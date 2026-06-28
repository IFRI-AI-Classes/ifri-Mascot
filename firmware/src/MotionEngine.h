#ifndef MOTION_ENGINE_H
#define MOTION_ENGINE_H

#include "ServoDriver.h"
#include "Constants.h"

// ======== MOTION MODES ========
enum MotionMode {
  MOTION_IDLE = 0,           // Arrêt
  MOTION_WALK_FORWARD = 1,   // Marche avant
  MOTION_WALK_BACKWARD = 2,  // Marche arrière
  MOTION_TURN_LEFT = 3,      // Virage gauche
  MOTION_TURN_RIGHT = 4,     // Virage droite
  MOTION_ROLL = 5            // Roulis (basculement)
};

// ======== GAIT PHASE ========
enum GaitPhase {
  GAIT_PHASE_1 = 0,  // Jambe gauche avance, pied droit stabilise
  GAIT_PHASE_2 = 1,  // Jambe gauche revient, pieds se centrent
  GAIT_PHASE_3 = 2,  // Jambe droite avance, pied gauche stabilise
  GAIT_PHASE_4 = 3   // Jambe droite revient, pieds se centrent
};

// ======== FUNCTIONS ========

/**
 * Initialise le moteur de mouvement
 */
void motionEngineInit();

/**
 * Met à jour le moteur de mouvement
 * À appeler continuellement (dans loop())
 */
void motionEngineUpdate();

/**
 * Démarre un mouvement spécifique
 * @param mode: Mode de mouvement (MOTION_WALK_FORWARD, etc.)
 * @param speed: Vitesse (WALK_SPEED_SLOW, WALK_SPEED_NORMAL, WALK_SPEED_FAST)
 */
void motionStart(MotionMode mode, uint16_t speed);

/**
 * Arrête le mouvement en cours
 */
void motionStop();

/**
 * Retourne le mode de mouvement actuel
 */
MotionMode motionGetCurrentMode();

/**
 * Retourne si le robot est en mouvement
 */
bool motionIsMoving();

/**
 * Lance une séquence d'accueil (animation)
 */
void motionGreetSequence();

/**
 * Lance une animation de danse
 */
void motionDanceSequence();

// ======== PRIVATE HELPER FUNCTIONS ========

/**
 * Exécute la phase actuelle de la marche
 */
void executeGaitPhase();

/**
 * Marche avant
 */
void executeWalkForward();

/**
 * Marche arrière
 */
void executeWalkBackward();

/**
 * Virage gauche
 */
void executeTurnLeft();

/**
 * Virage droite
 */
void executeTurnRight();

/**
 * Roulis (basculement)
 */
void executeRoll();

#endif
