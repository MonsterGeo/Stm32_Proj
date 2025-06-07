

#ifndef Timer
#define Timer
#include "stm32f10x.h"
void Timer_Init(void);
void TIM2_IRQHandler(void);
void IR_Receive();
#endif