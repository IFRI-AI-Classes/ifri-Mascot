#include "BatteryDriver.h"

static BatteryDriver batteryDriver(BATTERY_PIN);

BatteryDriver::BatteryDriver(uint8_t pin)
  : _pin(pin),
    _voltage(0.0f),
    _percentage(0),
    _isLow(false),
    _isCritical(false),
    _index(0),
    _lastUpdateMs(0) {
  for (uint8_t i = 0; i < BATTERY_SAMPLE_COUNT; i++) {
    _readings[i] = 0;
  }
}

void BatteryDriver::begin() {
  pinMode(_pin, INPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(_pin, ADC_11db);

  uint16_t firstReading = readAdcMillivolts();
  for (uint8_t i = 0; i < BATTERY_SAMPLE_COUNT; i++) {
    _readings[i] = firstReading;
  }

  _index = 0;
  _lastUpdateMs = 0;
  readVoltage();
  printStatus();
}

void BatteryDriver::update() {
  unsigned long now = millis();
  if (now - _lastUpdateMs < BATTERY_UPDATE_MS) {
    return;
  }

  _lastUpdateMs = now;
  readVoltage();
}

float BatteryDriver::readVoltage() {
  pushReading(readAdcMillivolts());

  float adcVoltage = averageReading() / 1000.0f;
  float batteryVoltage = adcVoltage * BATTERY_DIVIDER_RATIO;
  refreshState(batteryVoltage);

  return _voltage;
}

float BatteryDriver::getVoltage() const {
  return _voltage;
}

uint8_t BatteryDriver::getPercentage() const {
  return _percentage;
}

bool BatteryDriver::isLow() const {
  return _isLow;
}

bool BatteryDriver::isCritical() const {
  return _isCritical;
}

void BatteryDriver::printStatus() const {
  Serial.print("[Battery] ");
  Serial.print(_voltage, 2);
  Serial.print("V | ");
  Serial.print(_percentage);
  Serial.print("% | ");

  if (_isCritical) {
    Serial.println("CRITIQUE");
  } else if (_isLow) {
    Serial.println("FAIBLE");
  } else {
    Serial.println("OK");
  }
}

uint16_t BatteryDriver::readAdcMillivolts() const {
  return static_cast<uint16_t>(analogReadMilliVolts(_pin));
}

void BatteryDriver::pushReading(uint16_t millivolts) {
  _readings[_index] = millivolts;
  _index = (_index + 1) % BATTERY_SAMPLE_COUNT;
}

uint16_t BatteryDriver::averageReading() const {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < BATTERY_SAMPLE_COUNT; i++) {
    sum += _readings[i];
  }

  return static_cast<uint16_t>(sum / BATTERY_SAMPLE_COUNT);
}

void BatteryDriver::refreshState(float voltage) {
  _voltage = voltage;

  if (_voltage <= BATTERY_MIN_VOLTAGE) {
    _percentage = 0;
  } else if (_voltage >= BATTERY_MAX_VOLTAGE) {
    _percentage = 100;
  } else {
    float normalized = (_voltage - BATTERY_MIN_VOLTAGE) /
                       (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE);
    _percentage = static_cast<uint8_t>(normalized * 100.0f + 0.5f);
  }

  _isLow = _voltage <= BATTERY_LOW_VOLTAGE;
  _isCritical = _voltage <= BATTERY_CRITICAL_VOLTAGE;
}

void batteryDriverInit() {
  batteryDriver.begin();
}

void batteryDriverUpdate() {
  batteryDriver.update();
}

float batteryGetVoltage() {
  return batteryDriver.getVoltage();
}

uint8_t batteryGetPercent() {
  return batteryDriver.getPercentage();
}

bool batteryIsLow() {
  return batteryDriver.isLow();
}

bool batteryIsCritical() {
  return batteryDriver.isCritical();
}

void batteryPrintStatus() {
  batteryDriver.printStatus();
}
