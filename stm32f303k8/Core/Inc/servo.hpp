#ifndef SERVO_H
#define SERVO_H

#include"tim.h"
#include<math.h>

#define psc 10000
#define duty_max 0.1
#define duty_min 0.05
struct servo_num{
    TIM_HandleTypeDef *time;
    uint8_t channel;
    /* data */
};

void Servo_Start(servo_num s,int rad);
#endif

