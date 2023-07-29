#ifndef WALK
#define WALK
#include<unistd.h>
#include<servo.hpp>
#include<vector>

class walk{
    public:
        walk();
        void init();
        int  walking(servo *leg,int theta);
        void set_leg_num(servo* leg,int ownrad);
        std::vector<servo>legs;
        int num_legs=0;
        int x,y,z;
        int init_x,init_y,init_z;
        int offset_y,offset_x,offset_z,old_rad;
        
}; 

#endif 
  
