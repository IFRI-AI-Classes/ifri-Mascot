#include "BatterySafety.h"
#include "BatteryDriver.h"
#include "Constants.h"

static bool safetyActive = false;
static bool safetyCritical = false;

void batterySafetyUpdate() {
  uint8_t percent = batteryGetPercent();
  bool wasActive = safetyActive;

  safetyActive = batteryIsLow() || percent <= BATTERY_SAFETY_PERCENT;
  safetyCritical = batteryIsCritical() || percent <= BATTERY_CRITICAL_PERCENT;

  if (safetyActive && !wasActive) {
    Serial.println("[BatterySafety] Batterie faible: vitesse limitee");
    // TODO: brancher audioPlayLowBattery() quand AudioDriver sera pret.
  }
}

bool batterySafetyIsActive() {
  return safetyActive;
}

bool batterySafetyIsCritical() {
  return safetyCritical;
}

bool batterySafetyCanUseHighPowerActions() {
  return !safetyActive && !safetyCritical;
}

uint16_t batterySafetyLimitSpeed(uint16_t requestedSpeed) {
  if (!safetyActive) {
    return requestedSpeed;
  }

  return max<uint16_t>(requestedSpeed, WALK_SPEED_SLOW);
}

const char* batterySafetyGetState() {
  if (safetyCritical) {
    return "critical";
  }

  if (safetyActive) {
    return "limited";
  }

  return "normal";
}
