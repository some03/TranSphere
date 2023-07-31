#include<walk.hpp>
#include<math.h>
#include<climits>
/*
void walk::set_leg_num(servo* leg,int ownrad){
        
    bool set_flag=1;
    int i=0;
    int j=0;

    while(set_flag){
        for(int i=0;i<num_legs;i++){
            if(leg[i].position==ownrad){
                if(!set_flag)break;
                leg[i].relative_pos=j;
                set_flag=0;
            }else{
                leg[i].relative_pos=j;
                j++;
            }
            legs.push_back(leg[j]);
        }
    }
}
*/
int walk::walking(servo *legs,int rad){
    
    
    num_legs=sizeof(legs)/sizeof(legs[0]);
    int8_t x=1*cos(rad);
    int8_t y=1*sin(rad);
    /**
    if((!direction1)==0&&(!direction2==0)){
        //進行方向が斜めのときは機体を進行方向に向ける
        int nrad=INT_MAX;
        for(int i=0;i<num_legs;i++)nrad=std::min(nrad,abs(legs[i].position-rad)); 
        for(int i=0;i<num_legs;i++){
            legs[i].move((x+offset_x*cos(nrad))/2,(y+offset_y*sin(nrad))/2,z+offset_z);
            legs[i].move(x+offset_x*cos(nrad),y+offset_y*sin(nrad),z);
            legs[i].move(x,y,z);
        }
    }
    */
    
    //if(rad!=old_rad)set_leg_num(leg,rad);
   //initialize legs position------------- 
    init_x=1/2;
    init_y=1/2;
    init_z=0;
    legs[0].move(init_x,init_y,init_z); 
    legs[1].move(init_x,init_y,init_z); 
    legs[2].move(init_x,init_y,init_z); 
    legs[3].move(init_x,init_y,init_z); 
  //------------------------------------
    legs[0].move(x,(2*y+offset_y)/2,z+offset_z); 
    legs[0].move(x,y+offset_y,z); 
    for(int i=0;i<num_legs;i++)legs[i].move(x,(2*y-offset_y)/2,z+offset_z),legs[i].move(x,y-offset_y,z);
    for(int i=2;i>0;i--)legs[i].move(x,(2*y+offset_y)/2,z+offset_z),legs[i].move(x,y+offset_y,z);
    for(int i=0;i<num_legs;i++)legs[i].move(x,(2*y+offset_y)/2,z-offset_z),legs[i].move(x,y-offset_y,z);
    legs[3].move(x,y+offset_y,z);
    
    old_rad=rad;
    return 0;
}
void spraddle_legs(servo_num s,int rad){

}

