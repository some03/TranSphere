#include "servo.hpp"
#include "tim.h"
void Servo_Start(servo_num s,int rad){

    float p=((rad/M_PI)+duty_min)*psc;
    if(p>duty_max)p=duty_max;
    __HAL_TIM_SET_COMPARE(s.time,s.channel,p);
}