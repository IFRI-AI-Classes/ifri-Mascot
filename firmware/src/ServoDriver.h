#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#define NB_SERVOS 4


//strcture pour pouvoir creer des objets servos plus tard
struct Servo {
    unsigned int gpio;
    int angleActuel;
    int angleCible;
};

void servoDriverInit();
void servoDriverUpdate();
int servoDriverGetAngle(unsigned int servoId);
void servoDriverSetAngle(unsigned int servoId, int angle);

#endif
