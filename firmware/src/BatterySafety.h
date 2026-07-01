#ifndef BATTERY_SAFETY_H
#define BATTERY_SAFETY_H

#include <Arduino.h>

void batterySafetyUpdate();
bool batterySafetyIsActive();
bool batterySafetyIsCritical();
bool batterySafetyCanUseHighPowerActions();
uint16_t batterySafetyLimitSpeed(uint16_t requestedSpeed);
const char* batterySafetyGetState();

#endif
