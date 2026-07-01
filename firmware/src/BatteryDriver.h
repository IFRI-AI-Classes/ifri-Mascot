#ifndef BATTERY_DRIVER_H
#define BATTERY_DRIVER_H

#include <Arduino.h>
#include "Constants.h"
#include "Pinout.h"

class BatteryDriver {
public:
  explicit BatteryDriver(uint8_t pin = BATTERY_PIN);

  void begin();
  void update();

  float readVoltage();
  float getVoltage() const;
  uint8_t getPercentage() const;
  bool isLow() const;
  bool isCritical() const;
  void printStatus() const;

private:
  uint8_t _pin;
  float _voltage;
  uint8_t _percentage;
  bool _isLow;
  bool _isCritical;
  uint8_t _index;
  uint16_t _readings[BATTERY_SAMPLE_COUNT];
  unsigned long _lastUpdateMs;

  uint16_t readAdcMillivolts() const;
  void pushReading(uint16_t millivolts);
  uint16_t averageReading() const;
  void refreshState(float voltage);
};

void batteryDriverInit();
void batteryDriverUpdate();
float batteryGetVoltage();
uint8_t batteryGetPercent();
bool batteryIsLow();
bool batteryIsCritical();
void batteryPrintStatus();

#endif
