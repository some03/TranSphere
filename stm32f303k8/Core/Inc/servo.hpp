#ifndef SERVO_H
#define SERVO_H

#include"tim.h"
#include<math.h>

//servo moter SG90
#define psc 10000
#define t_max 2.4*0.001 //0.0024s 0deg
#define t_min 0.5*0.001 //0,0005s -90deg
#define t_pwm 0.02 //50Hz 0,02s
struct servo_num{
    TIM_HandleTypeDef *time;
    uint8_t channel;
    /* data */
};

void Servo_Start(servo_num s,float rad);
#endif

