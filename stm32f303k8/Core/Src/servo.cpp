#include "servo.hpp"

void Servo_Start(servo_num s,int rad){

    float p=((rad/M_PI)+duty_min)*psc;
    if(p>duty_max)p=duty_max;
    PWM_Start(s.timer,s.channel,p);
}