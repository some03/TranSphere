#ifndef CAN_H
#define CAN_H
#include<stdint.h>

typedef struct _SYSTEMInit{
   uint8_t pll_mlutiple;
   uint8_t apb1_ps;
   uint8_t latency;
}SYSTEMInit,*PSystemInit;

typedef struct _CanInit{
    uint8_t mcr_time_triger;
    uint8_t mcr_bussoff;
    uint8_t mcr_wakeup;
    uint8_t mcr_retransmit;
    uint8_t mcr_fifo_lock;
    uint8_t mcr_fifo_priority;
    
    uint8_t btr_debug_silent;
    uint8_t btr_debug_loopback;
    uint8_t btr_prescalar;
    uint8_t btr_swj;
    uint8_t btr_tseg1;
    uint8_t btr_tseg2;
}CanInit, *PCanInit;

typedef struct _CanFilterInit{
   uint8_t fm1r_filtermode;
   uint8_t filter_scale;
   uint8_t fm1r_fifo_number;
   uint8_t f0r1_device_id1;
   uint8_t f0r1_device_id2;
   uint8_t f0r1_device_id3;
   uint8_t f0r1_device_id4;
}CanFilterInit, *PCanFilterInit;

typedef struct _CanTxHeader{
   uint32_t transmit_id_StdId;
   uint32_t transmit_datasize_DLC;
   uint32_t transmit_grobaltime;
   int64_t  transmit_data;
}CanTxHeader, *PCanTxHeader;

typedef struct _CanRxHeader{
   uint32_t receive_id_StdId;
   uint32_t receive_rtr;
   uint32_t receive_datasize_DLC;
   uint32_t receive_timestamp;
   uint32_t receive_filtermatchindex;
   int64_t  receive_data;
}CanRxHeader, *PCanRxHeader;

uint32_t system_init(SYSTEMInit &sysinit);
uint32_t can_init(CanInit &caninit);
uint32_t can_filter_init(CanFilterInit &canfilterinit);
uint32_t can_get_mailboxfreelevel();
int32_t can_add_txmessage(CanTxHeader &cantxhed);
int32_t can_get_rxmessage(CanRxHeader &canrxhed,CanFilterInit canfilinit);

#endif
