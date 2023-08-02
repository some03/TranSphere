#ifndef TIMER_H
#define TIMER_H
#include<stdint.h>
#include "stm32f3xx.h"
void Timer1_PWM_Init(uint16_t count_max,int16_t prescaler);
void Timer2_PWM_Init(uint16_t count_max, int16_t prescaler);
void Timer3_PWM_Init(uint16_t count_max, int16_t prescaler);
void Timer16_PWM_Init(uint16_t count_max,int16_t prescaler);
void Timer17_PWM_Init(uint16_t count_max,int16_t prescaler);
void PWM_Start(int8_t timer, int8_t ch, int16_t duty);
#endif