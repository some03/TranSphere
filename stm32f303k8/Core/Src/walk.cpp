#include<walk.hpp>
#include<math.h>
#include<climits>
void walk::set_leg_num(servo* leg,int rad){
        
    bool set_flag=1;
    int i=0;
    int j=0;

    while(set_flag){
        for(int i=0;i<num_legs;i++){
            if(leg[i].position==rad){
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

int walk::walking(servo *leg,int rad){
    
    num_legs=sizeof(leg)/sizeof(leg[0]);
    int8_t direction1=1*cos(rad);
    int8_t direction2=1*sin(rad);
    if((!direction1)==0&&(!direction2==0)){
        //$B?J9TJ}8~$,<P$a$N$H$-$O5!BN$r?J9TJ}8~$K8~$1$k(B
        int nrad=INT_MAX;
        for(int i=0;i<num_legs;i++)nrad=std::min(nrad,abs(legs[i].position-rad)); 
        for(int i=0;i<num_legs;i++){
            legs[i].move((x+offset_x*cos(nrad))/2,(y+offset_y*sin(nrad))/2,z+offset_z);
            legs[i].move(x+offset_x*cos(nrad),y+offset_y*sin(nrad),z);
            legs[i].move(x,y,z);
        }
    }
    
    if(rad!=old_rad)set_leg_num(leg,rad);
   //initialize legs position------------- 
    legs[0].move(x,y,z); 
    legs[1].move(x,y,z); 
    legs[2].move(x,y,z); 
    legs[3].move(x,y,z); 
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

