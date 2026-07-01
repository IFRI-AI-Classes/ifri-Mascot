#include "UltrasonicDriver.h"
#include "Constants.h"
#include "Pinout.h"
#include <Arduino.h>

static volatile uint32_t echoStartUs = 0;
static volatile uint32_t echoEndUs = 0;
static volatile bool echoWaitingForFall = false;
static volatile bool echoSampleReady = false;

static portMUX_TYPE echoMux = portMUX_INITIALIZER_UNLOCKED;

static float readings[ULTRASONIC_SAMPLE_COUNT];
static uint8_t readingIndex = 0;
static uint8_t readingCount = 0;
static float distanceCm = 0.0f;
static bool hasValidMeasure = false;
static unsigned long lastTriggerMs = 0;

static bool isDistanceInRange(float value) {
    return value >= ULTRASONIC_MIN_DISTANCE_CM && value <= ULTRASONIC_MAX_DISTANCE_CM;
}

static void pushReading(float value) {
    readings[readingIndex] = value;
    readingIndex = (readingIndex + 1) % ULTRASONIC_SAMPLE_COUNT;

    if (readingCount < ULTRASONIC_SAMPLE_COUNT) {
        readingCount++;
    }
}

static float averageReading() {
    if (readingCount == 0) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (uint8_t i = 0; i < readingCount; i++) {
        sum += readings[i];
    }

    return sum / readingCount;
}

static void IRAM_ATTR handleEchoChange() {
    uint32_t nowUs = micros();

    portENTER_CRITICAL_ISR(&echoMux);
    if (digitalRead(ULTRASONIC_ECHO_PIN) == HIGH) {
        echoStartUs = nowUs;
        echoWaitingForFall = true;
    } else if (echoWaitingForFall) {
        echoEndUs = nowUs;
        echoWaitingForFall = false;
        echoSampleReady = true;
    }
    portEXIT_CRITICAL_ISR(&echoMux);
}

static void triggerMeasure() {
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
}

void ultrasonicDriverInit() {
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);

    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    for (uint8_t i = 0; i < ULTRASONIC_SAMPLE_COUNT; i++) {
        readings[i] = 0.0f;
    }

    attachInterrupt(digitalPinToInterrupt(ULTRASONIC_ECHO_PIN), handleEchoChange, CHANGE);

    Serial.println("[UltrasonicDriver] HC-SR04 initialise");
}

void ultrasonicDriverUpdate() {
    unsigned long nowMs = millis();
    uint32_t startUs = 0;
    uint32_t endUs = 0;
    bool sampleReady = false;
    bool timedOut = false;

    if (nowMs - lastTriggerMs >= ULTRASONIC_PERIOD_MS) {
        lastTriggerMs = nowMs;
        triggerMeasure();
    }

    portENTER_CRITICAL(&echoMux);
    if (echoSampleReady) {
        startUs = echoStartUs;
        endUs = echoEndUs;
        echoSampleReady = false;
        sampleReady = true;
    } else if (echoWaitingForFall && (micros() - echoStartUs > ULTRASONIC_TIMEOUT_US)) {
        echoWaitingForFall = false;
        timedOut = true;
    }
    portEXIT_CRITICAL(&echoMux);

    if (timedOut) {
        hasValidMeasure = false;
        return;
    }

    if (!sampleReady) {
        return;
    }

    uint32_t durationUs = endUs - startUs;
    float measuredCm = (durationUs * 0.0343f) / 2.0f;

    if (!isDistanceInRange(measuredCm)) {
        hasValidMeasure = false;
        return;
    }

    pushReading(measuredCm);
    distanceCm = averageReading();
    hasValidMeasure = true;
}

float ultrasonicDriverGetDistance() {
    return ultrasonicDriverGetDistanceCm();
}

float ultrasonicDriverGetDistanceCm() {
    return distanceCm;
}

bool ultrasonicDriverHasValidMeasure() {
    return hasValidMeasure;
}

bool ultrasonicDriverIsObstacleDetected() {
    return hasValidMeasure && distanceCm <= OBSTACLE_DISTANCE_MIN;
}
