#include "MotionEngine.h"

static MotionMode currentMotionMode = MOTION_IDLE;
static uint16_t motionSpeed = WALK_SPEED_NORMAL;
static bool isMoving = false;

static void setSupportDriveMode() {
  servoSetAngle(SERVO_LEFT_SUPPORT, SUPPORT_LEFT_DRIVE_ANGLE);
  servoSetAngle(SERVO_RIGHT_SUPPORT, SUPPORT_RIGHT_DRIVE_ANGLE);
}

static void setSupportRestMode() {
  servoSetAngle(SERVO_LEFT_SUPPORT, SUPPORT_LEFT_REST_ANGLE);
  servoSetAngle(SERVO_RIGHT_SUPPORT, SUPPORT_RIGHT_REST_ANGLE);
}

static void setWheelCommand(uint8_t leftCommand, uint8_t rightCommand) {
  servoSetAngleImmediate(SERVO_LEFT_WHEEL, leftCommand);
  servoSetAngleImmediate(SERVO_RIGHT_WHEEL, rightCommand);
}

static uint8_t forwardCommandForSpeed(uint16_t speed) {
  if (speed <= WALK_SPEED_FAST) {
    return WHEEL_SERVO_FORWARD;
  }
  if (speed >= WALK_SPEED_SLOW) {
    return WHEEL_SERVO_STOP + ((WHEEL_SERVO_FORWARD - WHEEL_SERVO_STOP) / 2);
  }
  return WHEEL_SERVO_FORWARD;
}

static uint8_t backwardCommandForSpeed(uint16_t speed) {
  if (speed <= WALK_SPEED_FAST) {
    return WHEEL_SERVO_BACKWARD;
  }
  if (speed >= WALK_SPEED_SLOW) {
    return WHEEL_SERVO_STOP - ((WHEEL_SERVO_STOP - WHEEL_SERVO_BACKWARD) / 2);
  }
  return WHEEL_SERVO_BACKWARD;
}

void motionEngineInit() {
  Serial.println("[MotionEngine] Initialisation du moteur roues/pattes...");
  motionStop();
  Serial.println("[MotionEngine] Pret");
}

void motionEngineUpdate() {
  // Les servos 360 continuent de tourner tant que leur consigne reste active.
}

void motionStart(MotionMode mode, uint16_t speed) {
  if (mode == MOTION_IDLE) {
    motionStop();
    return;
  }

  currentMotionMode = mode;
  motionSpeed = speed;
  isMoving = true;
  setSupportDriveMode();

  switch (mode) {
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
      motionStop();
      return;
  }

  Serial.print("[MotionEngine] Mouvement continu: ");
  Serial.print(mode);
  Serial.print(", vitesse ");
  Serial.println(speed);
}

void motionStop() {
  isMoving = false;
  currentMotionMode = MOTION_IDLE;
  setWheelCommand(WHEEL_SERVO_STOP, WHEEL_SERVO_STOP);
  setSupportRestMode();
  Serial.println("[MotionEngine] Arret du mouvement");
}

MotionMode motionGetCurrentMode() {
  return currentMotionMode;
}

bool motionIsMoving() {
  return isMoving;
}

void executeGaitPhase() {
  // Conserve pour compatibilite avec l'ancien prototype bipede.
}

void executeWalkForward() {
  uint8_t command = forwardCommandForSpeed(motionSpeed);
  setWheelCommand(command, WHEEL_SERVO_BACKWARD);
}

void executeWalkBackward() {
  uint8_t command = backwardCommandForSpeed(motionSpeed);
  setWheelCommand(command, WHEEL_SERVO_FORWARD);
}

void executeTurnLeft() {
  setWheelCommand(WHEEL_SERVO_BACKWARD, WHEEL_SERVO_BACKWARD);
}

void executeTurnRight() {
  setWheelCommand(WHEEL_SERVO_FORWARD, WHEEL_SERVO_FORWARD);
}

void executeRoll() {
  executeWalkForward();
}

void motionGreetSequence() {
  Serial.println("[MotionEngine] Sequence d'accueil");
  motionStop();
}

void motionDanceSequence() {
  Serial.println("[MotionEngine] Sequence de danse");
  motionStart(MOTION_ROLL, WALK_SPEED_NORMAL);
}
