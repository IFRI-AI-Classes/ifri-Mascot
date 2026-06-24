#include "ServoDriver.h"
#include "Pinout.h"
#include "driver/ledc.h"
#include <Arduino.h>


#define PWM_FREQ 50
#define PWM_RESOLUTION 10
#define PULSATION_MIN 1000 // 1ms pour 0degre
#define PULSATION_MAX 2000 // 2ms pour 180
#define PERIODE_DE_SIGNAL 20000 // 20ms

Servo servos[NB_SERVOS];

static int conversion(int angle){

    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    float pulse = PULSATION_MIN + (angle / 180.0) * (PULSATION_MAX - PULSATION_MIN);
    int duty = (pulse / PERIODE_DE_SIGNAL) * 1023; // 1023 = 2^10-1;
    return duty;
}

void servoDriverInit() {

  // innitialisation des servos
  servos[0].gpio = SERVO_JAMBE_GAUCHE_PIN;
  servos[0].canal = 0;
  servos[0].angleActuel = 90;
  servos[0].angleCible = 90;

  servos[1].gpio = SERVO_JAMBE_DROITE_PIN;
  servos[1].canal = 1;
  servos[1].angleActuel = 90;
  servos[1].angleCible = 90;

  servos[2].gpio = SERVO_PIED_GAUCHE_PIN;
  servos[2].canal = 2;
  servos[2].angleActuel = 90;
  servos[2].angleCible = 90;

  servos[3].gpio = SERVO_PIED_DROIT_PIN;
  servos[3].canal = 3;
  servos[3].angleActuel = 90;
  servos[3].angleCible = 90;

  //configuration des servos
  ledcSetup(0, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(SERVO_JAMBE_GAUCHE_PIN, 0);

  ledcSetup(1, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(SERVO_JAMBE_DROITE_PIN, 1);

  ledcSetup(2, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(SERVO_PIED_GAUCHE_PIN, 2);

  ledcSetup(3, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(SERVO_PIED_DROIT_PIN, 3);
}

void servoDriverUpdate(){
    for (int i = 0; i < NB_SERVOS; i++) {
        int duty = conversion(servos[i].angleCible);
        ledcWrite(servos[i].canal, duty);
        servos[i].angleActuel = servos[i].angleCible;
    }
}

int servoDriverGetAngle(unsigned int servoId) {
    if (servoId >= NB_SERVOS) return -1;
    return servos[servoId].angleActuel;
}

void servoDriverSetAngle(unsigned int servoId, int angle){
    if (servoId >= NB_SERVOS) return;
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    servos[servoId].angleCible = angle;
}
