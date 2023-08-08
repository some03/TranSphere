#include "servo.hpp"
#include "tim.h"
#include <stdio.h>
void Servo_Start(servo_num s,int rad){

    float p=((rad/M_PI)+duty_min)*psc;
    if(p>psc)p=psc;
    __HAL_TIM_SET_COMPARE(s.time,s.channel,(int)p);
}