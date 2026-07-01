#include "UltrasonicDriver.h"
#include "Pinout.h"
#include <Arduino.h>

#define ULTRASONIC_PERIOD_MS 60

static volatile unsigned long debutEcho = 0;
static volatile unsigned long finEcho = 0;
static volatile bool nouvelleMesureDisponible = false;

static float distanceCm = 0.0f;
static unsigned long precedentMillis = 0;

static void IRAM_ATTR gestionnaireEcho(){
    if (digitalRead(ULTRASONIC_ECHO_PIN) == HIGH){
        debutEcho = micros();
    }
    else{
        finEcho = micros();
        nouvelleMesureDisponible = true;
    }
}

void ultrasonicDriverInit(){
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);

    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    attachInterrupt(digitalPinToInterrupt(ULTRASONIC_ECHO_PIN), gestionnaireEcho, CHANGE);
}

void ultrasonicDriverUpdate(){
    unsigned long now = millis();

    // Déclenche une mesure toutes les 60 ms
    if (now - precedentMillis >= ULTRASONIC_PERIOD_MS){
        precedentMillis = now;

        digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
        delayMicroseconds(2);

        digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
        delayMicroseconds(10);

        digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    }

    // Traitement du résultat mesuré par l'ISR
    if (nouvelleMesureDisponible){
        unsigned long duree = finEcho - debutEcho;
        distanceCm = (duree * 0.0343f) / 2.0f;
        nouvelleMesureDisponible = false;
    }
}

float ultrasonicDriverGetDistance(){
    return distanceCm;
}
