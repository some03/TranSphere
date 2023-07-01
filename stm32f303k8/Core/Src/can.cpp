#include<stdint.h>
#include"can.hpp"
#include"stm32f3xx.h"
#define bits 1
//settings----------------
//HSI 8MHz
//PLL ×16
//system clock 64MHz
//APB1 prescalar 2
//flash acr latency 2
//APB1 clock 32MHz
//
//CAN prescalar 4 (time quantum 125ns)
//TSeg1=11 TSeg2=4 sampling point 75per

uint32_t system_init(SYSTEMInit &sysinit){
    
    RCC->CR&=(~bits)<<24;//PLLON disable
    while(RCC->CR&(bits<<25));//wait PLLRDY crear
    RCC->CFGR&=(~bits)<<16;//PLL input clock HSI/2
    RCC->CFGR|=sysinit.apb1_ps<<10;//APB1 prescalar 
    RCC->CFGR|=sysinit.pll_mlutiple<<21;//PLL 遁数倍

    FLASH->ACR|=sysinit.latency;//データ読み込み時遅延
                     
    RCC->CR|=bits<<24;//PLLON enable
    while(!(RCC->CR&(bits<<25)));//wait until PLL is ready
    RCC->CFGR|=bits<<1;//SYSCLK=PLL
    while(!(RCC->CFGR&(bits<<1)));//wait until PLL clock supply starts
    
}


uint32_t can_init(CanInit &caninit){
    CAN->MCR|=bits;//start can initialization
    while(!(CAN->MSR&1));
   
    CAN->MCR|=caninit.mcr_time_triger<<7;//ttmc
    CAN->MCR|=caninit.mcr_bussoff<<6;//auto bussoff abom
    CAN->MCR|=caninit.mcr_wakeup<<5;//auto wakeup mode awum
    CAN->MCR|=caninit.mcr_retransmit<<4;//auto retransmitt nart
    CAN->MCR|=caninit.mcr_fifo_lock<<3;//fifo lock mode rflm
    CAN->MCR|=caninit.mcr_fifo_priority<<2;//fifo priority txfp


    CAN->BTR|=caninit.btr_debug_silent<<31;//debug silent mode 
    CAN->BTR|=caninit.btr_debug_loopback<<30;//debug loop mode
    CAN->BTR|=caninit.btr_prescalar;//can prescalar    
    CAN->BTR|=caninit.btr_swj<24;//SWJ    
    CAN->BTR|=caninit.btr_tseg2<<20;//TSeg2    
    CAN->BTR|=caninit.btr_tseg1<<16;//TSeg1    

    CAN->MCR&=~bits;//finish can initialization
    while((CAN->MSR&1));

}

uint32_t can_filter_init(CanFilterInit &canfilinit){
    CAN->FMR|=bits;//filtter initalaize mode
    CAN->FA1R&=~bits;//disenable filter 0
    CAN->FM1R|=canfilinit.fm1r_filtermode;//filtermode 0==mask 1==list
    uint8_t fifo=canfilinit.fm1r_filtermode;
    if(canfilinit.filter_scale==16){
        CAN->FS1R&=~bits;//filter scale 16bit mode
        CAN->sFilterRegister[fifo].FR1|=(canfilinit.f0r1_device_id1<<5)<<15; 
        CAN->sFilterRegister[fifo].FR1|=(canfilinit.f0r1_device_id2<<5); 
        CAN->sFilterRegister[fifo].FR2|=(canfilinit.f0r1_device_id3<<5)<<15; 
        CAN->sFilterRegister[fifo].FR2|=(canfilinit.f0r1_device_id4<<5); 
    }
    else if(canfilinit.filter_scale==32){
        CAN->FS1R|=bits;//filter scale 32bit mode 
        CAN->sFilterRegister[fifo].FR1|=(canfilinit.f0r1_device_id1<21);
        CAN->sFilterRegister[fifo].FR2|=(canfilinit.f0r1_device_id2<<21);
    }

    if(canfilinit.fm1r_filtermode)CAN->FFA1R|=bits;//FIFO1 is selected
    else  CAN->FFA1R&=~bits;//FIFO0 is selected
    CAN->FA1R|=bits;//enable filter
}

uint32_t can_get_mailboxfreelevel(){
    uint8_t freelevel=0;
    if(CAN->TSR&(bits<<26))freelevel++;
    if(CAN->TSR&(bits<<27))freelevel++;
    if(CAN->TSR&(bits<<28))freelevel++;
    return freelevel;
}

int32_t can_add_txmessage(CanTxHeader &cantxhed){
    
    uint8_t mailbox=can_get_mailboxfreelevel();
    uint8_t transmitmailbox=0b11&(CAN->TSR>>25);//chose mail box

    CAN->sTxMailBox[transmitmailbox].TIR=cantxhed.transmit_id_StdId<<21;//set stId
    CAN->sTxMailBox[transmitmailbox].TDTR|=cantxhed.transmit_datasize_DLC;//setting data length
    if(cantxhed.transmit_grobaltime)CAN->sTxMailBox[transmitmailbox].TDTR|=bits<<8;//enable transmit grobaltime
    CAN->sTxMailBox[transmitmailbox].TDLR=cantxhed.transmit_data&0xFFFFFFFF;//set low data
    CAN->sTxMailBox[transmitmailbox].TDHR=cantxhed.transmit_data>>0xFFFFFFFF;//set low data
    CAN->sTxMailBox[transmitmailbox].TIR=bits;//request transmit

    return 0;
}

int32_t can_get_rxmessage(CanRxHeader &canrxhed,CanFilterInit canfilinit){
    
    if(canfilinit.fm1r_filtermode){
        if(!(CAN->RF1R&0b01))return -1;// mail is not received
    }else{
        if(!(CAN->RF0R&0b01))return -1;// mail is not received
    }

    canrxhed.receive_rtr=CAN->sFIFOMailBox[canfilinit.fm1r_filtermode].RIR&0b1;//get remot transmit request
    canrxhed.receive_datasize_DLC=CAN->sFIFOMailBox[canfilinit.fm1r_filtermode].RDTR&0b111;//get  data length
    canrxhed.receive_filtermatchindex=(CAN->sFIFOMailBox[canfilinit.fm1r_filtermode].RDTR>>8)&0xFF;//get  filter match index
    canrxhed.receive_timestamp=CAN->sFIFOMailBox[canfilinit.fm1r_filtermode].RDTR>>16;//get   message time stamp
    
    canrxhed.receive_data=CAN->sFIFOMailBox[canfilinit.fm1r_filtermode].RDHR<<0xFFFFFFFF;//read data HIGH
    canrxhed.receive_data|=CAN->sFIFOMailBox[canfilinit.fm1r_filtermode].RDLR;//read data LOW

    if(canfilinit.fm1r_filtermode)CAN->RF1R&=~bits<<5;
    else CAN->RF0R&=~bits<<5;//release mail box

    return 0;
}



