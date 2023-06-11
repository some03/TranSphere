#include<servo.hpp>

using namespace std;
servo::servo(int pin_[3], bool position_){
    for(int i=0;i<3;i++)pin[i]=pin_[i];
    position=position_;
}
void servo::init(){
    //servo1 init
    //servo2 init
    //servo0 init
}
void servo::move(double x,double y,double z){
   
    s1=(int)(((pow(r1,2)+pow(af,2)-pow(r2,2))/(2*r1*af))-atan2(d,h-z));
    //servo1 move
    s2=(int)((pow(r1,2)+pow(r2,2)-pow(af,2))/(2*r1*r2));
    //servo2 move
    s0=(int)atan2(x,y);
    //servo3 move
}
