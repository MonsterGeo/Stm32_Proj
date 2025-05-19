#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "stm32f10x.h"

#define Size 200

typedef struct {
	uint8_t head; //写指针
	uint8_t tail; 
	uint8_t Length;
	uint8_t Ring_Buff[Size];
} RingBuff_t, Q;
extern Q q;
uint8_t Write_RB(uint16_t data);
uint16_t Read_RB(uint16_t *rData);
void RB_Init(void);

#endif

