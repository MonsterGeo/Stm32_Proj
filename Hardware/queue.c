#include "stm32f10x.h"
#include <stdio.h>
#include "queue.h"



void Q_Init(Q* q){
	q->head = q->tail = 0;
}
uint8_t isFull(Q *q){
	return ((q->tail+1 ) % Buffer_Size == q->head);
}

uint8_t isEmpty(Q *q){
	return (q->head == q->tail);
}

uint8_t Q_write(Q* q,uint16_t data ){
	if(isFull(q)){
		return 0;
	}
	q->buffer[q->head] = data;
	q->head = (q->head+1) % Buffer_Size;
	return 1;
}

uint8_t Q_Read(Q *q, uint8_t* data){
	if(isEmpty(q)){
		return 0;
	}
	*data = q->buffer[q->tail];
	q->tail = (q->tail+1) % Buffer_Size;
	return 1;
}