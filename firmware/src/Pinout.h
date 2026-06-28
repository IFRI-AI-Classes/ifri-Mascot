#ifndef PINOUT_H
#define PINOUT_H

// ======== SERVO PINS (PWM) ========
#define SERVO_LEFT_LEG_PIN   13   // Jambe gauche (hanche)
#define SERVO_RIGHT_LEG_PIN  12   // Jambe droite (hanche)
#define SERVO_LEFT_FOOT_PIN  14   // Pied gauche (équilibre)
#define SERVO_RIGHT_FOOT_PIN 27   // Pied droit (équilibre)

// ======== ULTRASONIC SENSOR ========
#define ULTRASONIC_TRIG_PIN  5    // Capteur HC-SR04 Trigger
#define ULTRASONIC_ECHO_PIN  18   // Capteur HC-SR04 Echo

// ======== AUDIO/SPEAKER ========
#define AUDIO_PIN            25   // Speaker (PWM compatible)

// ======== BATTERY/ADC ========
#define BATTERY_PIN          35   // Entrée ADC pour batterie (pin entrée uniquement)

// ======== LED/INDICATOR ========
#define STATUS_LED_PIN       2    // LED d'état

#endif
