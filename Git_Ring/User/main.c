#include "stm32f10x.h"
#include "Serial.h"
#include "queue.h"


//函数定义
void gpio_pin_13_init();
void Serial_Send(uint16_t Data);
void usart1_init();

uint16_t TData;
uint8_t RxData;
//变量定义
uint8_t Flag_Done;//串口传输标志位，传输完毕置1.

int main(){
	Q q;//初始化环形队列结构体q
	Flag_Done = 0;//初始化标志
	usart1_init();
	RB_Init();
	
	while(1){
		//usart接收串口代码
		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
			RxData = USART_ReceiveData(USART1);
			Write_RB(RxData);
			Flag_Done = 1;
		}
		//发送缓冲区中的数据
		if(Flag_Done){
			Flag_Done = 0;
			Read_RB(&TData);
			Serial_Send(TData);
			}
		}

}


void Serial_Send(uint16_t Data){
	USART_SendData(USART1,Data);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET){
	}//等待标志位
}


void usart1_init()
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//PA9初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//选用引脚9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //时钟为50hz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA10引脚初始化为上拉输入
	
	
	//tx配置
	USART_InitTypeDef USART_InitStructure;					//定义结构体变量
	USART_InitStructure.USART_BaudRate = 9600;				//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
	USART_Init(USART1, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
	
		
	/*中断输出配置*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//选择配置NVIC的USART1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
	
	
	USART_Cmd(USART1,ENABLE);
}