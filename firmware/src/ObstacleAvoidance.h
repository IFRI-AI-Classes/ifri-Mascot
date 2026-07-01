#ifndef OBSTACLE_AVOIDANCE_H
#define OBSTACLE_AVOIDANCE_H

#include <Arduino.h>

void obstacleAvoidanceInit();
void obstacleAvoidanceUpdate();
void obstacleAvoidanceCancel();
bool obstacleAvoidanceIsActive();
bool obstacleAvoidanceIsEnabled();
void obstacleAvoidanceSetEnabled(bool enabled);
const char* obstacleAvoidanceGetState();

#endif
