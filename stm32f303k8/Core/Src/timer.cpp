#include "timer.hpp"

void Timer1_PWM_Init(uint16_t count_max,int16_t prescaler)
{

  TIM1->ARR = count_max;   // set pwm frequency
  TIM1->PSC = prescaler;   // prescaler (64)

  TIM1->CCMR1 |= (0b0110 << 14); // CCMR1 OC2M pwm mode1
  TIM1->CCMR1 |= (0b0110 << 6);  // CCMR1 OC1M pwm mode1
  TIM1->CCMR2 |= (0b0110 << 14); // CCMR2 OC2M pwm mode1
  TIM1->CCMR2 |= (0b0110 << 6);  // CCMR2 OC1M pwm mode1

  TIM1->CCMR1 |= (0b1 << 11); // CCMR1 OC2PE enable
  TIM1->CCMR1 |= (0b1 << 3);  // CCMR1 OC1PE enable
  TIM1->CCMR2 |= (0b1 << 11); // CCMR2 OC2PE enable
  TIM1->CCMR2 |= (0b1 << 3);  // CCMR2 OC1PE enable

  TIM1->CR1 |= (0b1 << 7); // ARPE auto preloard enbale
  TIM1->EGR |= 0b1;        // set UG counter reset

  // TIM1->CCER|=0b1;//CH1 enable
  // TIM1->CCER|=(0b1<<4);//CH2 enable
  // TIM1->CCER|=(0b1<<8);//CH3 enable
  // TIM1->CCER|=(0b1<<12);//CH4 enable

  TIM1->CR1 |= 0b1; // timer2 enable
}

void Timer2_PWM_Init(uint16_t count_max, int16_t prescaler)
{

  TIM2->ARR = count_max;   // set pwm frequency
  TIM2->PSC = prescaler;   // prescaler (64)

  TIM2->CCMR1 |= (0b0110 << 14); // CCMR1 OC2M pwm mode1
  TIM2->CCMR1 |= (0b0110 << 6);  // CCMR1 OC1M pwm mode1
  TIM2->CCMR2 |= (0b0110 << 14); // CCMR2 OC2M pwm mode1
  TIM2->CCMR2 |= (0b0110 << 6);  // CCMR2 OC1M pwm mode1

  TIM2->CCMR1 |= (0b1 << 11); // CCMR1 OC2PE enable
  TIM2->CCMR1 |= (0b1 << 3);  // CCMR1 OC1PE enable
  TIM2->CCMR2 |= (0b1 << 11); // CCMR2 OC2PE enable
  TIM2->CCMR2 |= (0b1 << 3);  // CCMR2 OC1PE enable

  TIM2->CR1 |= (0b1 << 7); // ARPE auto preloard enbale
  TIM2->EGR |= 0b1;        // set UG counter reset

  // TIM2->CCER|=0b1;//CH1 enable
  // TIM2->CCER|=(0b1<<4);//CH2 enable
  // TIM2->CCER|=(0b1<<8);//CH3 enable
  // TIM2->CCER|=(0b1<<12);//CH4 enable

  TIM2->CR1 |= 0b1; // timer2 enable
}

void Timer3_PWM_Init(uint16_t count_max, int16_t prescaler)
{

  TIM3->ARR = count_max;   // set pwm frequency
  TIM3->PSC = prescaler;   // prescaler (64)

  TIM3->CCMR1 |= (0b0110 << 14); // CCMR1 OC2M pwm mode1
  TIM3->CCMR1 |= (0b0110 << 6);  // CCMR1 OC1M pwm mode1
  TIM3->CCMR2 |= (0b0110 << 14); // CCMR2 OC2M pwm mode1
  TIM3->CCMR2 |= (0b0110 << 6);  // CCMR2 OC1M pwm mode1

  TIM3->CCMR1 |= (0b1 << 11); // CCMR1 OC3PE enable
  TIM3->CCMR1 |= (0b1 << 3);  // CCMR1 OC1PE enable
  TIM3->CCMR3 |= (0b1 << 11); // CCMR3 OC3PE enable
  TIM3->CCMR3 |= (0b1 << 3);  // CCMR3 OC1PE enable

  TIM3->CR1 |= (0b1 << 7); // ARPE auto preloard enbale
  TIM3->EGR |= 0b1;        // set UG counter reset

  // TIM3->CCER|=0b1;//CH1 enable
  // TIM3->CCER|=(0b1<<4);//CH3 enable
  // TIM3->CCER|=(0b1<<8);//CH3 enable
  // TIM3->CCER|=(0b1<<13);//CH4 enable

  TIM3->CR1 |= 0b1; // timer3 enable
}

void Timer16_PWM_Init(uint16_t count_max,int16_t prescaler)
{

  TIM16->ARR = count_max;   // set pwm frequency
  TIM16->PSC = prescaler;   // prescaler (64)

  TIM16->CCMR1 |= (0b0110 << 14); // CCMR1 OC2M pwm mode1
  TIM16->CCMR1 |= (0b0110 << 6);  // CCMR1 OC1M pwm mode1
  TIM16->CCMR2 |= (0b0110 << 14); // CCMR2 OC2M pwm mode1
  TIM16->CCMR2 |= (0b0110 << 6);  // CCMR2 OC1M pwm mode1

  TIM16->CCMR1 |= (0b1 << 11); // CCMR1 OC3PE enable
  TIM16->CCMR1 |= (0b1 << 3);  // CCMR1 OC1PE enable
  TIM16->CCMR3 |= (0b1 << 11); // CCMR3 OC3PE enable
  TIM16->CCMR3 |= (0b1 << 3);  // CCMR3 OC1PE enable

  TIM16->CR1 |= (0b1 << 7); // ARPE auto preloard enbale
  TIM16->EGR |= 0b1;        // set UG counter reset

  // TIM4->CCER|=0b1;//CH1 enable
  // TIM4->CCER|=(0b1<<4);//CH3 enable
  // TIM4->CCER|=(0b1<<8);//CH3 enable
  // TIM4->CCER|=(0b1<<13);//CH4 enable

  TIM16->CR1 |= 0b1; // timer3 enable
}

void Timer17_PWM_Init(uint16_t count_max,int16_t prescaler)
{

  TIM17->ARR = count_max;   // set pwm frequency
  TIM17->PSC = prescaler;   // prescaler (64)

  TIM17->CCMR1 |= (0b0110 << 14); // CCMR1 OC2M pwm mode1
  TIM17->CCMR1 |= (0b0110 << 6);  // CCMR1 OC1M pwm mode1
  TIM17->CCMR2 |= (0b0110 << 14); // CCMR2 OC2M pwm mode1
  TIM17->CCMR2 |= (0b0110 << 6);  // CCMR2 OC1M pwm mode1

  TIM17->CCMR1 |= (0b1 << 11); // CCMR1 OC3PE enable
  TIM17->CCMR1 |= (0b1 << 3);  // CCMR1 OC1PE enable
  TIM17->CCMR3 |= (0b1 << 11); // CCMR3 OC3PE enable
  TIM17->CCMR3 |= (0b1 << 3);  // CCMR3 OC1PE enable

  TIM17->CR1 |= (0b1 << 7); // ARPE auto preloard enbale
  TIM17->EGR |= 0b1;        // set UG counter reset

  // TIM4->CCER|=0b1;//CH1 enable
  // TIM4->CCER|=(0b1<<4);//CH3 enable
  // TIM4->CCER|=(0b1<<8);//CH3 enable
  // TIM4->CCER|=(0b1<<13);//CH4 enable

  TIM17->CR1 |= 0b1; // timer3 enable
}
void PWM_Start(int8_t timer, int8_t ch, int16_t duty)
{

  if (timer == 1)
  {
    if (ch == 1)
      TIM1->CCR1 = duty; // PA8
    else if (ch == 2)
      TIM1->CCR2 = duty; // PA9
    else if (ch == 3)
      TIM1->CCR3 = duty; // PA10
    else
      TIM2->CCR1 = 0, TIM1->CCR2 = 0, TIM1->CCR3 = 0;
  }
  if (timer == 2)
  {
    if (ch == 1)
      TIM2->CCR1 = duty; // PA0
    else if (ch == 2)
      TIM2->CCR2 = duty; // PA1
    else if (ch == 3)
      TIM2->CCR3 = duty; // PA9
    else if (ch == 4)
      TIM2->CCR4 = duty; // PA10
    else
      TIM2->CCR1 = 0, TIM2->CCR2 = 0, TIM2->CCR3 = 0, TIM2->CCR4 = 0;
  }

  if (timer == 3)
  {
    if (ch == 1)
      TIM3->CCR1 = duty; // PA6
    else if (ch == 2)
      TIM3->CCR2 = duty; // PC7
    else if (ch == 3)
      TIM3->CCR3 = duty; // PC8
    else if (ch == 3)
      TIM3->CCR4 = duty; // PB7
    else
      TIM3->CCR1 = 0, TIM3->CCR2 = 0, TIM3->CCR3 = 0, TIM3->CCR4 = 0;
  }
  if (timer == 16)
  {
    TIM16->CCR1 = duty; // PB4
  }
}