#ifndef _queue_H_
#define _queue_H_
#include "stm32f10x.h"

#define Buffer_Size 256

typedef struct {
	uint8_t buffer[Buffer_Size];
	uint8_t head; //写指针
	uint8_t tail; //读指针
} Q;

void Q_Init(Q* q);
void Q_Init(Q* q);
uint8_t Q_write(Q* q,uint16_t data );
uint8_t Q_Read(Q *q, uint8_t* data);

#endif

