#include"can.h"
#include<iostream>
#define TRNSMITTER
//#define RECEIVER
void can_transmitdata(uint32_t stdid,uint32_t dlc,uint32_t grobaltime,int64_t data){
   CanTxHeader cantxheader; 
   cantxheader.transmit_id_StdId=stdid;
   cantxheader.transmit_datasize_DLC=dlc;
   cantxheader.transmit_grobaltime=grobaltime;
   cantxheader.transmit_data=data;

   can_add_txmessage(cantxheader);
   while(can_get_mailboxfreelevel()!=3);//wait until transmit fiish
}
void can_receivedata(uint32_t stdid,uint32_t rtr,uint32_t dlc,uint32_t timestamp,uint32_t matchindex,int64_t data,CanFilterInit canfinit){
    CanRxHeader canrxheader; 
    canrxheader.receive_id_StdId=stdid;
    canrxheader.receive_rtr=rtr;
    canrxheader.receive_datasize_DLC=dlc;
    canrxheader.receive_timestamp=timestamp;
    canrxheader.receive_filtermatchindex=matchindex;
    canrxheader.receive_data=data;

    can_get_rxmessage(canrxheader,canfinit);

}
int main(){
   
    SYSTEMInit systeminit;
    systeminit.pll_mlutiple=0b1110;
    systeminit.apb1_ps=0b100;
    systeminit.latency=0b010;
    system_init(systeminit);

    CanInit caninit;
    caninit.mcr_time_triger=0;//disable time triger mode
    caninit.mcr_bussoff=0;//disable auto bussoff
    caninit.mcr_wakeup=0;//disable auto wakeup
    caninit.mcr_retransmit=0;//enable retransmit
    caninit.mcr_fifo_lock=1;//enable fifo lockmode
    caninit.mcr_fifo_priority=1;//Priority is determined by the order of requests (0: identifier of message)
    caninit.btr_debug_silent=0;
    caninit.btr_debug_loopback=0;
    caninit.btr_debug_loopback=0;
    caninit.btr_prescalar=0b100;
    caninit.btr_swj=1;
    caninit.btr_tseg1=0b1011;
    caninit.btr_tseg2=0b100;
    can_init(caninit);

#ifdef TRANSMITTER
    uint32_t id=3;
    uint32_t datasize=8;
    uint32_t gtime=0;
    int64_t data=32;

    if(datasize>8)datasize=8;
    while(1){
        can_transmitdata(id,datasize,gtime,data); 
    } 
#endif
#ifdef RECEIVER
    uint32_t id=3;
    uint32_t rtr=0;
    uint32_t datasize=8;
    uint32_t timestamp=0;
    uint32_t matchindex=0;
    int64_t data=-1;

    while(1){
        can_receivedata(id,rtr,datasize,timstamp,matchindex,data);
        std::cout<<data<<endl;
    }
#endif


    
}
