#ifndef ULTRASONIC_DRIVER_H
#define ULTRASONIC_DRIVER_H

void ultrasonicDriverInit();
void ultrasonicDriverUpdate();

float ultrasonicDriverGetDistance();
float ultrasonicDriverGetDistanceCm();
bool ultrasonicDriverHasValidMeasure();
bool ultrasonicDriverIsObstacleDetected();

#endif
