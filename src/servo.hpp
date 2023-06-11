#ifndef SERVO
#define SERVO
#include<iostream>
#include<math.h>

class  servo{
    public:
        servo(){;}
        servo(int pin_[3],bool front);
        void move(double x,double y ,double z);
        void init();
        int s0,s1,s2;
        int pin[3],front;
        double d=1,af=1, h, z;
        float r1,r2;
};
#endif
