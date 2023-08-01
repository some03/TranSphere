#include"stm32f3xx.h"

void Timer1_PWM_Init(int16_t counter_max,int16_t prescaler);
void Timer2_PWM_Init(int16_t counter_max,int16_t prescaler);
void Timer3_PWM_Init(int16_t counter_max,int16_t prescaler);
void Timer16_PWM_Init(int16_t counter_max,int16_t prescaler);
void Timer17_PWM_Init(int16_t counter_max,int16_t prescaler);

void PWM_Start(int8_t tm,int8_t ch,int16_t duty);