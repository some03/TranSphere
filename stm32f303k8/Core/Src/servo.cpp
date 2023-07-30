#include<servo.hpp>
#include"stm32f3xx.h"
#include"timer.hpp"

#define psc 10000
servo::servo( servo_num* s){
    servo0=s[0];
    servo1=s[1];
    servo2=s[2];
}
void servo::init(){
    //servo1 init
    //servo2 init
    //servo0 init
}
void servo::move(double x,double y,double z){
    
    float duty_max=0.1;
    float duty_min=0.05;
    s1=(int)(((pow(r1,2)+pow(af,2)-pow(r2,2))/(2*r1*af))-atan2(d,h-z));
    //servo1 move
    int d1=((s1/M_PI)+duty_min)*psc;
    if(d1>duty_max)d1=duty_max;
    PWM_Start(servo1.timer,servo1.channel,d1);
    
    s2=(int)((pow(r1,2)+pow(r2,2)-pow(af,2))/(2*r1*r2));
    //servo2 move
    int d2=((s2/M_PI)+duty_min)*psc;
    if(d2>duty_max)d2=duty_max;
    PWM_Start(servo2.timer,servo2.channel,d2);
    
    //servo0 move
    if(position==1|position==3){
        s0=-(int)atan2(x,y);
        int d0=((s0/M_PI)+duty_min)*psc;
        if(d0>duty_max)d0=duty_max;
        PWM_Start(servo0.timer,servo0.channel,d0);
    }else{
        s0=(int)atan2(x,y);
        int d0=((s0/M_PI)+duty_min)*psc;
        if(d0>duty_max)d0=duty_max;
        PWM_Start(servo0.timer,servo0.channel,s0);
    }
}
