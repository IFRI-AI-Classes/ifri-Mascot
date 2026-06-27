#include "ServoDriver.h"
#include "Pinout.h"
#include <Arduino.h>

#define PWM_FREQ 50 // 50 hz
#define PWM_RESOLUTION 10 // 10 bits

#define PULSATION_MIN 500 // 5 ms pour 0 degre
#define PULSATION_MAX 2500 // 2,5 ms pour 180
#define PERIODE_DE_SIGNAL 20000 // 20ms periode du signal

Servo servos[NB_SERVOS];

// fonction pour convertir l'angle qu'on veut en signal comprehensible par le servo
static int conversion(int angle){
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    float pulse = PULSATION_MIN + (angle / 180.0f) * (PULSATION_MAX - PULSATION_MIN);
    int maxDuty = (1 << PWM_RESOLUTION) - 1;
    return (pulse / PERIODE_DE_SIGNAL) * maxDuty;
}

void servoDriverInit(){

    // innitialisation des servos
    servos[0].gpio = SERVO_JAMBE_GAUCHE_PIN;
    servos[0].angleActuel = 90;
    servos[0].angleCible = 90;

    servos[1].gpio = SERVO_JAMBE_DROITE_PIN;
    servos[1].angleActuel = 90;
    servos[1].angleCible = 90;

    servos[2].gpio = SERVO_PIED_GAUCHE_PIN;
    servos[2].angleActuel = 90;
    servos[2].angleCible = 90;

    servos[3].gpio = SERVO_PIED_DROIT_PIN;
    servos[3].angleActuel = 90;
    servos[3].angleCible = 90;

    //configuration des servos
    ledcAttach(SERVO_JAMBE_GAUCHE_PIN, PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(SERVO_JAMBE_DROITE_PIN, PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(SERVO_PIED_GAUCHE_PIN, PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(SERVO_PIED_DROIT_PIN, PWM_FREQ, PWM_RESOLUTION);

    servoDriverUpdate();
}

// Met à jour tous les servos
//Cette fonction doit être appelée régulièrement pour que les mouvements s'exécutent. Idéalement dans loop().
void servoDriverUpdate(){
    for (int i = 0; i < NB_SERVOS; i++){
        int duty = conversion(servos[i].angleCible);
        ledcWrite(servos[i].gpio, duty);
        servos[i].angleActuel = servos[i].angleCible;
    }
}

//Définit l'angle cible d'un servo
int servoDriverGetAngle(unsigned int servoId){
    if (servoId >= NB_SERVOS) return -1;
    return servos[servoId].angleActuel;
}

//Récupère l'angle actuel d'un servo
void servoDriverSetAngle(unsigned int servoId, int angle){
    if (servoId >= NB_SERVOS) return;
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    servos[servoId].angleCible = angle;
}
