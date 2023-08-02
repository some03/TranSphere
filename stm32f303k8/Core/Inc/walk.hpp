#ifndef WALK_H
#define WALK_H
#include<unistd.h>
#include"servo.hpp"
#include"tr_legs.hpp"
#include<vector>

class walk{
    public:
        walk(tr_legs *leg_,servo_num s);
        int  walking(int theta);
        void spraddle_legs(int rad);
        void set_leg_num(tr_legs *leg,int ownrad);
        std::vector<tr_legs>legs;
        servo_num center_s;
        int num_legs=0;
        int x,y,z;
        int init_x,init_y,init_z;
        int offset_y,offset_x,offset_z,old_rad;
        
}; 

#endif 
  
