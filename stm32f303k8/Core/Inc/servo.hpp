#ifndef SERVO
#define SERVO
#include<math.h>
#include<algorithm>
#include<stdint.h>
class  servo{
    public:
        servo(){;}
        servo(servo_num* s);
        void move(double x,double y ,double z);
        void init();
        int s0,s1,s2;
        int pin[3],position,relative_pos;
        double d=1,af=1, h, z;
        float r1,r2;
        servo_num servo0;
        servo_num servo1;
        servo_num servo2;
};
struct servo_num
{
    int timer;
    int channel;
    /* data */
};

#endif
