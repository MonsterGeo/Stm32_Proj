#include "stm32f10x.h"
#include "Serial.h"
#include "queue.h"
#include "Timer.h"





void Pin_IR_init(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

int main(){
	Pin_IR_init();
	while(1){
		
	};		
}