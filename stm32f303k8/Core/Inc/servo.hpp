#ifndef SERVO
#define SERVO
#include<math.h>
#include<algorithm>
#include<stdint.h>
class  servo{
    public:
        servo(){;}
        servo(int pin_[3],int8_t positon_);
        void move(double x,double y ,double z);
        void init();
        int s0,s1,s2;
        int pin[3],position,relative_pos;
        double d=1,af=1, h, z;
        float r1,r2;
};
#endif
