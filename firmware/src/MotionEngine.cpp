#include "MotionEngine.h"

// ======== STATE VARIABLES ========
MotionMode currentMotionMode = MOTION_IDLE;
GaitPhase currentGaitPhase = GAIT_PHASE_1;
uint16_t motionSpeed = WALK_SPEED_NORMAL;
unsigned long lastPhaseChangeTime = 0;
bool isMoving = false;

// ======== INITIALIZATION ========

void motionEngineInit() {
  Serial.println("[MotionEngine] Initialisation du moteur de mouvement...");
  servoDriverInit();
  motionStop();
  Serial.println("[MotionEngine] Prêt!");
}

void motionEngineUpdate() {
  if (!isMoving) {
    return;
  }
  
  // Vérifier si le temps pour changer de phase est écoulé
  unsigned long currentTime = millis();
  if (currentTime - lastPhaseChangeTime >= motionSpeed) {
    lastPhaseChangeTime = currentTime;
    
    // Avancer à la phase suivante
    currentGaitPhase = (GaitPhase)((currentGaitPhase + 1) % WALK_PHASE_COUNT);
    
    // Exécuter le mouvement correspondant à la phase
    executeGaitPhase();
  }
  
  // Mettre à jour les servos (mouvement progressif)
  servoDriverUpdate();
}

// ======== MOTION CONTROL ========

void motionStart(MotionMode mode, uint16_t speed) {
  if (mode == MOTION_IDLE) {
    motionStop();
    return;
  }
  
  currentMotionMode = mode;
  motionSpeed = speed;
  isMoving = true;
  currentGaitPhase = GAIT_PHASE_1;
  lastPhaseChangeTime = millis();
  
  Serial.print("[MotionEngine] Démarrage: Mode ");
  Serial.print(mode);
  Serial.print(", Vitesse ");
  Serial.println(speed);
  
  // Exécuter la première phase
  executeGaitPhase();
}

void motionStop() {
  isMoving = false;
  currentMotionMode = MOTION_IDLE;
  servoSetNeutralPosition();
  Serial.println("[MotionEngine] Arrêt du mouvement");
}

MotionMode motionGetCurrentMode() {
  return currentMotionMode;
}

bool motionIsMoving() {
  return isMoving;
}

// ======== GAIT ALGORITHM - MARCHE BIPÈDE ========

/**
 * Exécute la phase actuelle de la marche
 * Algorithme de marche bipède pour 4 servos:
 * 
 * PHASE 1: Jambe gauche avance, pieds s'équilibrent
 *   - Jambe gauche: 120° (avant)
 *   - Jambe droite: 90° (neutre)
 *   - Pied gauche: 70° (incliné)
 *   - Pied droit: 110° (incliné - équilibre)
 *
 * PHASE 2: Jambes se centrent
 *   - Jambe gauche: 90° (neutre)
 *   - Jambe droite: 90° (neutre)
 *   - Pied gauche: 90° (neutre)
 *   - Pied droit: 90° (neutre)
 *
 * PHASE 3: Jambe droite avance, pieds s'équilibrent
 *   - Jambe gauche: 90° (neutre)
 *   - Jambe droite: 60° (avant)
 *   - Pied gauche: 110° (incliné - équilibre)
 *   - Pied droit: 70° (incliné)
 *
 * PHASE 4: Jambes se centrent (retour à phase 1)
 *   - Jambe gauche: 90° (neutre)
 *   - Jambe droite: 90° (neutre)
 *   - Pied gauche: 90° (neutre)
 *   - Pied droit: 90° (neutre)
 */

void executeGaitPhase() {
  switch (currentMotionMode) {
    case MOTION_WALK_FORWARD:
      executeWalkForward();
      break;
    
    case MOTION_WALK_BACKWARD:
      executeWalkBackward();
      break;
    
    case MOTION_TURN_LEFT:
      executeTurnLeft();
      break;
    
    case MOTION_TURN_RIGHT:
      executeTurnRight();
      break;
    
    case MOTION_ROLL:
      executeRoll();
      break;
    
    default:
      break;
  }
}

// ======== WALKING FORWARD ========

void executeWalkForward() {
  switch (currentGaitPhase) {
    case GAIT_PHASE_1:
      // Jambe gauche avance, équilibre sur pied droit
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_FORWARD);    // 120°
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_NEUTRAL);  // 90°
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_LEFT_TILT);     // 70° (incliné)
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_RIGHT_TILT);   // 110° (équilibre)
      break;
    
    case GAIT_PHASE_2:
      // Centrage des jambes
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_NEUTRAL);    // 90°
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_NEUTRAL);  // 90°
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_CENTER);        // 90°
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_CENTER);       // 90°
      break;
    
    case GAIT_PHASE_3:
      // Jambe droite avance, équilibre sur pied gauche
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_NEUTRAL);    // 90°
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_FORWARD);  // 60°
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_LEFT_TILT);     // 70° (équilibre)
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_RIGHT_TILT);   // 110° (incliné)
      break;
    
    case GAIT_PHASE_4:
      // Centrage (retour au neutre)
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_NEUTRAL);
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_NEUTRAL);
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_CENTER);
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_CENTER);
      break;
  }
}

// ======== WALKING BACKWARD ========

void executeWalkBackward() {
  // Marche arrière = inverser les jambes par rapport à la marche avant
  switch (currentGaitPhase) {
    case GAIT_PHASE_1:
      // Jambe gauche recule
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_BACK);        // 60°
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_NEUTRAL);   // 90°
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_LEFT_TILT);      // 70°
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_RIGHT_TILT);    // 110°
      break;
    
    case GAIT_PHASE_2:
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_NEUTRAL);
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_NEUTRAL);
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_CENTER);
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_CENTER);
      break;
    
    case GAIT_PHASE_3:
      // Jambe droite recule
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_NEUTRAL);
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_BACK);       // 120°
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_LEFT_TILT);       // 70°
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_RIGHT_TILT);     // 110°
      break;
    
    case GAIT_PHASE_4:
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_NEUTRAL);
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_NEUTRAL);
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_CENTER);
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_CENTER);
      break;
  }
}

// ======== TURNING ========

void executeTurnLeft() {
  // Tourner à gauche = jambe gauche se lève/recule, jambe droite avance
  switch (currentGaitPhase) {
    case GAIT_PHASE_1:
    case GAIT_PHASE_2:
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_BACK);
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_FORWARD);
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_LEFT_TILT);
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_CENTER);
      break;
    
    case GAIT_PHASE_3:
    case GAIT_PHASE_4:
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_NEUTRAL);
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_NEUTRAL);
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_CENTER);
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_CENTER);
      break;
  }
}

void executeTurnRight() {
  // Tourner à droite = jambe droite se lève/recule, jambe gauche avance
  switch (currentGaitPhase) {
    case GAIT_PHASE_1:
    case GAIT_PHASE_2:
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_FORWARD);
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_BACK);
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_CENTER);
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_RIGHT_TILT);
      break;
    
    case GAIT_PHASE_3:
    case GAIT_PHASE_4:
      servoSetAngle(SERVO_LEFT_LEG, WALK_LEFT_LEG_NEUTRAL);
      servoSetAngle(SERVO_RIGHT_LEG, WALK_RIGHT_LEG_NEUTRAL);
      servoSetAngle(SERVO_LEFT_FOOT, WALK_FOOT_CENTER);
      servoSetAngle(SERVO_RIGHT_FOOT, WALK_FOOT_CENTER);
      break;
  }
}

// ======== ROLLING (ROULIS) ========

void executeRoll() {
  // Roulis = basculement latéral progressif pour avancer
  switch (currentGaitPhase) {
    case GAIT_PHASE_1:
      // Basculer vers la droite
      servoSetAngle(SERVO_LEFT_LEG, 90 - ROLL_ANGLE_MAX);       // Jambe gauche levée
      servoSetAngle(SERVO_RIGHT_LEG, 90 + ROLL_ANGLE_MAX);      // Jambe droite levée
      servoSetAngle(SERVO_LEFT_FOOT, 90 - ROLL_ANGLE_MAX);
      servoSetAngle(SERVO_RIGHT_FOOT, 90 + ROLL_ANGLE_MAX);
      break;
    
    case GAIT_PHASE_2:
      // Neutre
      servoSetAngle(SERVO_LEFT_LEG, SERVO_CENTER);
      servoSetAngle(SERVO_RIGHT_LEG, SERVO_CENTER);
      servoSetAngle(SERVO_LEFT_FOOT, SERVO_CENTER);
      servoSetAngle(SERVO_RIGHT_FOOT, SERVO_CENTER);
      break;
    
    case GAIT_PHASE_3:
      // Basculer vers la gauche
      servoSetAngle(SERVO_LEFT_LEG, 90 + ROLL_ANGLE_MAX);
      servoSetAngle(SERVO_RIGHT_LEG, 90 - ROLL_ANGLE_MAX);
      servoSetAngle(SERVO_LEFT_FOOT, 90 + ROLL_ANGLE_MAX);
      servoSetAngle(SERVO_RIGHT_FOOT, 90 - ROLL_ANGLE_MAX);
      break;
    
    case GAIT_PHASE_4:
      // Neutre
      servoSetAngle(SERVO_LEFT_LEG, SERVO_CENTER);
      servoSetAngle(SERVO_RIGHT_LEG, SERVO_CENTER);
      servoSetAngle(SERVO_LEFT_FOOT, SERVO_CENTER);
      servoSetAngle(SERVO_RIGHT_FOOT, SERVO_CENTER);
      break;
  }
}

// ======== SEQUENCES/ANIMATIONS ========

void motionGreetSequence() {
  Serial.println("[MotionEngine] Séquence d'accueil");
  // Animation simple: lever une jambe et baisser
  for (int i = 0; i < 3; i++) {
    servoSetAngle(SERVO_LEFT_LEG, 60);
    delay(300);
    servoSetAngle(SERVO_LEFT_LEG, 90);
    delay(300);
  }
}

void motionDanceSequence() {
  Serial.println("[MotionEngine] Séquence de danse");
  motionStart(MOTION_ROLL, WALK_SPEED_NORMAL);
  // Continue until stopped externally
}
