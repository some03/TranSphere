#include <stdint.h>
#include "can.hpp"
#include "stm32f3xx.h"
#define bits 1
// settings----------------
// HSI 8MHz
// PLL ×16
// system clock 64MHz
// APB1 prescalar 2
// flash acr latency 2
// APB1 clock 32MHz
//
// CAN prescalar 4 (time quantum 125ns)
// TSeg1=11 TSeg2=4 sampling point 75per

uint32_t system_init(SYSTEMInit &sysinit)
{

    RCC->CR &= (~bits) << 24; // PLLON disable
    while (RCC->CR & (bits << 25))
        ;                                    // wait PLLRDY crear
    RCC->CFGR &= (~bits) << 16;              // PLL input clock HSI/2
    RCC->CFGR |= sysinit.apb1_ps << 10;      // APB1 prescalar
    RCC->CFGR |= sysinit.pll_mlutiple << 21; // PLL 遁数倍

    FLASH->ACR |= sysinit.latency; // データ読み込み時遅延

    RCC->CR |= bits << 24; // PLLON enable
    while (!(RCC->CR & (bits << 25)))
        ;                   // wait until PLL is ready
    RCC->CFGR |= bits << 1; // SYSCLK=PLL
    while (!(RCC->CFGR & (bits << 1))); // wait until PLL clock supply starts
}

void Can_Msp_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* CAN clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    NVIC_SetPriority(CAN_RX0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(CAN_RX0_IRQn);

    /* CAN interrupt Init */
}

uint32_t can_init(CanInit &caninit)
{
    CAN->MCR &= ~(0b10); // unlock sleep mode
    while (!(CAN->MSR & 0b10)){;} // wait until disable sleep mode
    CAN->MCR |= 0b1; // start can initialiation
    while (!(CAN->MSR)) {;} // wait until start initialization

    CAN->MCR |= caninit.mcr_time_triger << 7;   // ttmc
    CAN->MCR |= caninit.mcr_bussoff << 6;       // auto bussoff abom
    CAN->MCR |= caninit.mcr_wakeup << 5;        // auto wakeup mode awum
    CAN->MCR |= caninit.mcr_retransmit << 4;    // auto retransmitt nart
    CAN->MCR |= caninit.mcr_fifo_lock << 3;     // fifo lock mode rflm
    CAN->MCR |= caninit.mcr_fifo_priority << 2; // fifo priority txfp

    CAN->BTR |= caninit.btr_debug_silent << 31;   // debug silent mode
    CAN->BTR |= caninit.btr_debug_loopback << 30; // debug loop mode
    CAN->BTR |= caninit.btr_prescalar;            // can prescalar
    CAN->BTR |= caninit.btr_swj < 24;             // SWJ
    CAN->BTR |= caninit.btr_tseg2 << 20;          // TSeg2
    CAN->BTR |= caninit.btr_tseg1 << 16;          // TSeg1

    //CAN->IER|=(bits<<1);//interrupt when FMP!=0;

    CAN->MCR &= ~bits; // finish can initialization
    while ((CAN->MSR & 1))
    {
        ;
    }; // wait until can initializeation
}

uint32_t can_filter_init(CanFilterInit &canfilinit)
{
    CAN->FMR |= bits;                                      // start filtter initalaize mode
    CAN->FA1R &= ~(bits << canfilinit.fm1r_filter_number); // disenable filter 0

    CAN->FM1R |= (canfilinit.fm1r_filtermode << canfilinit.fm1r_filter_number); // filtermode 0==mask 1==list
    uint32_t filter = canfilinit.fm1r_filter_number;
    if (canfilinit.filter_scale == 16)
    {
        CAN->FS1R &= ~(bits << canfilinit.fm1r_filter_number); // filter scale 16bit mode
        CAN->sFilterRegister[filter].FR1 = (canfilinit.f0r1_device_id1 << 5);
        CAN->sFilterRegister[filter].FR1 = (canfilinit.f0r1_device_id2 << 21);
        CAN->sFilterRegister[filter].FR2 = (canfilinit.f0r1_device_id3 << 5);
        CAN->sFilterRegister[filter].FR2 = (canfilinit.f0r1_device_id4 << 21);
    }
    else if (canfilinit.filter_scale == 32)
    {
        CAN->FS1R |= (bits << canfilinit.fm1r_filter_number); // filter scale 32bit mode
        CAN->sFilterRegister[filter].FR1 |= (canfilinit.f0r1_device_id1 << 21);
        CAN->sFilterRegister[filter].FR2 |= (canfilinit.f0r1_device_id2 << 21);
    }

    if (canfilinit.fm1r_fifo_number)
        CAN->FFA1R |= (bits << canfilinit.fm1r_filter_number); // FIFO1 is selected
    else
        CAN->FFA1R &= ~(bits << canfilinit.fm1r_filter_number); // FIFO0 is selected

    CAN->FA1R |= (bits << canfilinit.fm1r_filter_number); // enable filter
    CAN->FMR &= ~bits;                                    // finish filtter initalaize mode
}

uint32_t can_get_mailboxfreelevel()
{
    uint8_t freelevel = 0;
    if (CAN->TSR & (bits << 26))
        freelevel++;
    if (CAN->TSR & (bits << 27))
        freelevel++;
    if (CAN->TSR & (bits << 28))
        freelevel++;
    return freelevel;
}

int32_t can_add_txmessage(CanTxHeader &cantxhed)
{

    uint8_t mailbox = can_get_mailboxfreelevel();
    uint8_t transmitmailbox = 0b11 & (CAN->TSR >> 24); // chose mail box

    CAN->sTxMailBox[transmitmailbox].TIR = (cantxhed.transmit_id_StdId) << 21; // set stId
    CAN->sTxMailBox[transmitmailbox].TDTR = cantxhed.transmit_datasize_DLC;    // setting data length
    if (cantxhed.transmit_grobaltime)
        CAN->sTxMailBox[transmitmailbox].TDTR |= bits << 8;                         // enable transmit grobaltime

    CAN->sTxMailBox[transmitmailbox].TDHR = (cantxhed.transmit_data[7] <<CAN_TDH0R_DATA7); // set low data
    CAN->sTxMailBox[transmitmailbox].TDHR = (cantxhed.transmit_data[6] <<CAN_TDH0R_DATA6); // set low data
    CAN->sTxMailBox[transmitmailbox].TDHR = (cantxhed.transmit_data[5] <<CAN_TDH0R_DATA5); // set low data
    CAN->sTxMailBox[transmitmailbox].TDHR = (cantxhed.transmit_data[4] <<CAN_TDH0R_DATA4); // set low data
    
    CAN->sTxMailBox[transmitmailbox].TDLR = (cantxhed.transmit_data[3] <<CAN_TDL0R_DATA3);  // set low data
    CAN->sTxMailBox[transmitmailbox].TDLR = (cantxhed.transmit_data[2] <<CAN_TDL0R_DATA2);  // set low data
    CAN->sTxMailBox[transmitmailbox].TDLR = (cantxhed.transmit_data[1] <<CAN_TDL0R_DATA1);  // set low data
    CAN->sTxMailBox[transmitmailbox].TDLR = (cantxhed.transmit_data[0] <<CAN_TDL0R_DATA0);  // set low data
    
    CAN->sTxMailBox[transmitmailbox].TIR = bits;                                    // request transmit

    return 0;
}

int32_t can_get_rxmessage(CanRxHeader &canrxhed, CanFilterInit canfilinit)
{

    if (canfilinit.fm1r_fifo_number)
    {
        if (!(CAN->RF1R & 0b01))
            return -1; // mail is not received
    }
    else
    {
        if (!(CAN->RF0R & 0b01))
            return -1; // mail is not received
    }

    canrxhed.receive_rtr = CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RIR & 0b1;                       // get remot transmit request
    canrxhed.receive_datasize_DLC = CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDTR & 0b111;           // get  data length
    canrxhed.receive_filtermatchindex = (CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDTR >> 8) & 0xFF; // get  filter match index
    canrxhed.receive_timestamp = CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDTR >> 16;                // get   message time stamp

    canrxhed.receive_data[0] = (CAN_RDL0R_DATA0&CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDLR)>>CAN_RDL0R_DATA0_Pos; // read data HIGH
    canrxhed.receive_data[1] = (CAN_RDL0R_DATA1&CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDLR)>>CAN_RDL0R_DATA1_Pos; // read data HIGH
    canrxhed.receive_data[2] = (CAN_RDL0R_DATA2&CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDLR)>>CAN_RDL0R_DATA2_Pos; // read data HIGH
    canrxhed.receive_data[3] = (CAN_RDL0R_DATA3&CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDLR)>>CAN_RDL0R_DATA3_Pos; // read data HIGH
    canrxhed.receive_data[4] = (CAN_RDH0R_DATA4&CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDLR)>>CAN_RDH0R_DATA4_Pos;              // read data LOW
    canrxhed.receive_data[5] = (CAN_RDH0R_DATA5&CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDLR)>>CAN_RDH0R_DATA5_Pos;              // read data LOW
    canrxhed.receive_data[6] = (CAN_RDH0R_DATA6&CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDLR)>>CAN_RDH0R_DATA6_Pos;              // read data LOW
    canrxhed.receive_data[7] = (CAN_RDH0R_DATA7&CAN->sFIFOMailBox[canfilinit.fm1r_fifo_number].RDLR)>>CAN_RDH0R_DATA7_Pos;              // read data LOW

    return 0;
}
