#ifndef IR_REC
#define IR_REC
#include "stm32f10x.h"

uint8_t Pin_IR;
uint8_t Flag_Fali_edge;
uint8_t Flag_IR_Head;

uint8_t CheckSum;
uint8_t  IR_Num;
uint32_t IR_Data[4];
uint32_t Data;
uint32_t R_Time_H;
uint32_t R_Time_L;


void IR_Receive(void);
#endif