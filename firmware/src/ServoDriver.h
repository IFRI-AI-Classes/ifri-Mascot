#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#define NB_SERVOS 4

//strcture pour pouvoir creer des objets servos plus tard
struct Servo {
    unsigned int gpio;
    unsigned int canal;
    int angleActuel;
    int angleCible;
};

void servoDriverInit();
void servoDriverUpdate();
int servoDriverGetAngle(unsigned int servoId);// obetenir la valeur de l'angle
void servoDriverSetAngle(unsigned int servoId, int angle);// modifier la valeur de l'angle

#endif
