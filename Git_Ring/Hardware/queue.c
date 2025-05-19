#include "stm32f10x.h"
#include "queue.h"
#include "Serial.h"



//=======================================================\

Q q;
// 链表初始化
void RB_Init(){
	q.head = 0;
	q.tail = 0;
	q.Length =0;
}


//把串口数据写入环形队列
uint8_t Write_RB(uint16_t data){
	if(q.Length >= Size){
		return 0;
	}
	q.Ring_Buff[q.tail] = data;
	q.tail = (q.tail+1) % Size;
	q.Length++;
	return 1;
}
//从环形队列中读数据
uint16_t Read_RB(uint16_t *rData){
	if(q.Length == 0){
		*rData=0;
		return 0;
	}
	*rData = q.Ring_Buff[q.head];
	q.head = (q.head + 1) % Size;
	q.Length--;
	return 1;
}