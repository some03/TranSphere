#ifndef WALK
#define WALK
#include <iostream>
#include <unistd.h>
#include <servo.hpp>

class walk{
    public:
        walk();
        void init();
        
        int  walking(servo leg[4],int z);
        
}; 

#endif 
  
