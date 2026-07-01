#ifndef PINOUT_H
#define PINOUT_H

// ======== SERVO PINS (PWM) ========
#define SERVO_LEFT_LEG_PIN   18
#define SERVO_RIGHT_LEG_PIN  19
#define SERVO_LEFT_FOOT_PIN  21
#define SERVO_RIGHT_FOOT_PIN 22

// Aliases utilises par la branche servo-driver.
#define SERVO_JAMBE_GAUCHE_PIN  SERVO_LEFT_LEG_PIN
#define SERVO_JAMBE_DROITE_PIN  SERVO_RIGHT_LEG_PIN
#define SERVO_PIED_GAUCHE_PIN   SERVO_LEFT_FOOT_PIN
#define SERVO_PIED_DROIT_PIN    SERVO_RIGHT_FOOT_PIN

// ======== ULTRASONIC SENSOR ========
#define ULTRASONIC_ECHO_PIN 14
#define ULTRASONIC_TRIG_PIN 15

// ======== AUDIO/SPEAKER ========
#define AUDIO_PIN 25

// ======== BATTERY/ADC ========
#define BATTERY_PIN 35

// ======== LED/INDICATOR ========
#define STATUS_LED_PIN 2

#endif
