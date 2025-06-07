#include "stm32f10x.h"
#include "Serial.h"
#include "Timer.h"
#define Pin_IR GPIO_Pin_10

void Timer_Init(){ //定时50us
	
	//初始化Tim2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 定时时间 = (重装载值 + 1) * (预分频值 + 1) / 定时器时钟频率
    // 50us = (ARR + 1) * (0 + 1) / 72MHz => ARR = 72MHz * 50us - 1 = 3600 - 1 = 3599
	TIM_TimeBaseInitStructure.TIM_Period = 35;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 99;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_Inits;
	NVIC_Inits.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Inits.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Inits.NVIC_IRQChannelPreemptionPriority  = 2;
	NVIC_Inits.NVIC_IRQChannelSubPriority  = 1;
	NVIC_Init(&NVIC_Inits);
	
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(){//TIm2中断函数
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		IR_Receive();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

typedef struct{
	uint8_t Flag_Fali_edge:1;
	uint8_t Flag_IR_Head:1;
	
}Flags;

uint8_t Flag_Fali_edge;
uint8_t IR_Num;
uint8_t Flag_IR_Head;
uint8_t CheckSum;
uint32_t R_Time_H;
uint32_t R_Time_L;
uint32_t Data;
uint32_t IR_Data[4];



void IR_Receive(){
    if(Pin_IR == 1){
        Flag_Fali_edge = 1; //下降沿标志位，1保持不变，0表示下降沿来到
        if(R_Time_H < 500){
            R_Time_H++; //高电平有效，自增
        }
    }
    else{
        if(Flag_Fali_edge){
            Flag_Fali_edge = 0;
            if(Flag_IR_Head == 0){
                if((R_Time_L > 140 && R_Time_L <230) && R_Time_H > 60){
                    Flag_IR_Head = 1;//头码验证通过，接收完毕
                    //开始初始化数据包相关变量
                    Data = 0;//接收用引脚数据
                    IR_Num = 0;//一位一位接收数据，该变量用来计算接收了多少 位数据
                    for(int i=0; i<5 ;i++){
											IR_Data[i] = 0;
										}
                }
                    //清零有效高低电平计数开始接收数据
    			    R_Time_L = 0;
    			    R_Time_H = 0;            
                }
                else{
                if(R_Time_L < 15 && R_Time_L > 5){
                    Data <<= 1;
                    if(R_Time_H > 20){
                    Data |= (0x01 << 0); //若检测到500us低电平+ 1500us高电平则将数据置1
                    }
                    IR_Num++; //接收完一位数据自增
                    if(IR_Num >= 32){ //一帧数据有32位
                        IR_Num = 0;
                        IR_Data[0] = Data & 0xff; //相当于将除了头8位数据   之外的 数字置0
                        IR_Data[1] = Data >> 8 & 0xff;//左移8位将4-8位数据  保存
                        IR_Data[2] = Data >> 16 & 0xff; //左移16位保存数据
                        IR_Data[3] = Data >> 24 & 0xff;
                        CheckSum = ~IR_Data[1];//校验
                    }
                    if(CheckSum == IR_Data[0]){
                        //接收完毕 串口输出到命令行
                        //Usart.Send(IR_Data);
                    }
            }
            else{ //不是数据包则清空包头接收变量
                Flag_IR_Head = 0;
            }
            //清空有效电平计数器
            R_Time_L = 0;
            R_Time_H = 0;
            }
        }
        else{
            if(R_Time_L < 500){
                R_Time_L++; //低电平有效
            }
        }
    }
}


