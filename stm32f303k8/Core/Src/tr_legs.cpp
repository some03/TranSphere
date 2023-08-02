#include"servo.hpp"
#include"tr_legs.hpp"
#include"stm32f3xx.h"
#include"timer.hpp"

tr_legs::tr_legs( servo_num* s){
    servo0=s[0];
    servo1=s[1];
    servo2=s[2];
}
void tr_legs::init(){
    //servo1 init
    //servo2 init
    //servo0 init
}
void tr_legs::move(double x,double y,double z){
    
    s1=(int)(((pow(r1,2)+pow(af,2)-pow(r2,2))/(2*r1*af))-atan2(d,h-z));
    //servo1 move
    Servo_Start(servo1,s1);
    
    s2=(int)((pow(r1,2)+pow(r2,2)-pow(af,2))/(2*r1*r2));
    //servo2 move
    Servo_Start(servo2,s2);
    
    //servo0 move
    if(position==1|position==3){
        s0=-(int)atan2(x,y);
        Servo_Start(servo0,s0);
    }else{
        Servo_Start(servo0,s0);
    }
}
