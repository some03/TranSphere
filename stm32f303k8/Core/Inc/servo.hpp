#ifndef SERVO_H
#define SERVO_H

#include"timer.hpp"
#include<math.h>

#define psc 10000
#define duty_max 0.1
#define duty_min 0.05
struct servo_num{
    int timer;
    int channel;
    /* data */
};

void Servo_Start(servo_num s,int rad);
#endif

