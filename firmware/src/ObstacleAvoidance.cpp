#include "ObstacleAvoidance.h"
#include "Constants.h"
#include "MotionEngine.h"
#include "UltrasonicDriver.h"

enum ObstacleAvoidanceState {
  OBSTACLE_AVOID_IDLE = 0,
  OBSTACLE_AVOID_STOPPING,
  OBSTACLE_AVOID_BACKING,
  OBSTACLE_AVOID_TURNING,
  OBSTACLE_AVOID_RECOVERING
};

static bool avoidanceEnabled = true;
static ObstacleAvoidanceState state = OBSTACLE_AVOID_IDLE;
static unsigned long stateStartedMs = 0;

static void enterState(ObstacleAvoidanceState nextState) {
  state = nextState;
  stateStartedMs = millis();

  switch (state) {
    case OBSTACLE_AVOID_STOPPING:
      motionStop();
      Serial.println("[ObstacleAvoidance] Obstacle detecte: stop");
      // TODO: brancher audioPlayObstacle() quand AudioDriver sera pret.
      break;
    case OBSTACLE_AVOID_BACKING:
      motionStart(MOTION_WALK_BACKWARD, OBSTACLE_AVOID_SPEED);
      Serial.println("[ObstacleAvoidance] Recul");
      break;
    case OBSTACLE_AVOID_TURNING:
      motionStart(MOTION_TURN_RIGHT, OBSTACLE_AVOID_SPEED);
      Serial.println("[ObstacleAvoidance] Rotation");
      break;
    case OBSTACLE_AVOID_RECOVERING:
      motionStop();
      Serial.println("[ObstacleAvoidance] Recuperation");
      break;
    case OBSTACLE_AVOID_IDLE:
    default:
      break;
  }
}

void obstacleAvoidanceInit() {
  avoidanceEnabled = true;
  state = OBSTACLE_AVOID_IDLE;
  stateStartedMs = 0;
}

void obstacleAvoidanceUpdate() {
  if (!avoidanceEnabled) {
    return;
  }

  unsigned long now = millis();

  if (state == OBSTACLE_AVOID_IDLE) {
    bool obstacleClose = ultrasonicDriverHasValidMeasure() &&
                         ultrasonicDriverGetDistanceCm() <= OBSTACLE_AVOID_TRIGGER_CM;

    if (motionIsMoving() && motionGetCurrentMode() == MOTION_WALK_FORWARD && obstacleClose) {
      enterState(OBSTACLE_AVOID_STOPPING);
    }
    return;
  }

  switch (state) {
    case OBSTACLE_AVOID_STOPPING:
      enterState(OBSTACLE_AVOID_BACKING);
      break;
    case OBSTACLE_AVOID_BACKING:
      if (now - stateStartedMs >= OBSTACLE_BACKUP_MS) {
        enterState(OBSTACLE_AVOID_TURNING);
      }
      break;
    case OBSTACLE_AVOID_TURNING:
      if (now - stateStartedMs >= OBSTACLE_TURN_MS) {
        enterState(OBSTACLE_AVOID_RECOVERING);
      }
      break;
    case OBSTACLE_AVOID_RECOVERING:
      if (now - stateStartedMs >= OBSTACLE_RECOVERY_MS) {
        enterState(OBSTACLE_AVOID_IDLE);
      }
      break;
    case OBSTACLE_AVOID_IDLE:
    default:
      break;
  }
}

void obstacleAvoidanceCancel() {
  if (state != OBSTACLE_AVOID_IDLE) {
    state = OBSTACLE_AVOID_IDLE;
    stateStartedMs = 0;
  }
}

bool obstacleAvoidanceIsActive() {
  return state != OBSTACLE_AVOID_IDLE;
}

bool obstacleAvoidanceIsEnabled() {
  return avoidanceEnabled;
}

void obstacleAvoidanceSetEnabled(bool enabled) {
  avoidanceEnabled = enabled;
  if (!avoidanceEnabled && state != OBSTACLE_AVOID_IDLE) {
    motionStop();
    enterState(OBSTACLE_AVOID_IDLE);
  }
}

const char* obstacleAvoidanceGetState() {
  switch (state) {
    case OBSTACLE_AVOID_STOPPING:
      return "stopping";
    case OBSTACLE_AVOID_BACKING:
      return "backing";
    case OBSTACLE_AVOID_TURNING:
      return "turning";
    case OBSTACLE_AVOID_RECOVERING:
      return "recovering";
    case OBSTACLE_AVOID_IDLE:
    default:
      return "idle";
  }
}
