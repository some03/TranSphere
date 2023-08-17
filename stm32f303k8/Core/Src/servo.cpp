#include "servo.hpp"
#include "tim.h"
#include <stdio.h>

void Servo_Start(servo_num s,float rad){

    float p=((((rad/M_PI)*(t_max-t_min))+t_min)/t_pwm)*psc;
    //printf("%f\n",p);
    
    if(p>(psc*(t_max/t_pwm)))p=(psc*(t_max/t_pwm));
    if(p<(psc*(t_min/t_pwm)))p=(psc*(t_min/t_pwm));
    
    __HAL_TIM_SET_COMPARE(s.time,s.channel,(int)p);
}