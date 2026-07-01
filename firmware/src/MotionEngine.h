#ifndef MOTION_ENGINE_H
#define MOTION_ENGINE_H

#include "ServoDriver.h"
#include "Constants.h"

enum MotionMode {
  MOTION_IDLE = 0,
  MOTION_WALK_FORWARD = 1,
  MOTION_WALK_BACKWARD = 2,
  MOTION_TURN_LEFT = 3,
  MOTION_TURN_RIGHT = 4,
  MOTION_ROLL = 5
};

enum GaitPhase {
  GAIT_PHASE_1 = 0,
  GAIT_PHASE_2 = 1,
  GAIT_PHASE_3 = 2,
  GAIT_PHASE_4 = 3
};

void motionEngineInit();
void motionEngineUpdate();

void motionStart(MotionMode mode, uint16_t speed);
void motionStop();

MotionMode motionGetCurrentMode();
bool motionIsMoving();

void motionGreetSequence();
void motionDanceSequence();

void executeGaitPhase();
void executeWalkForward();
void executeWalkBackward();
void executeTurnLeft();
void executeTurnRight();
void executeRoll();

#endif
