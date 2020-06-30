#include "timer.h" 
#include "LQ12864.h"
#include "stdio.h"
Status Light ;                               //定义交通灯的状态
uint16_t second = 60 ;                       //时间，单位是秒
uint16_t foul_people = 0 ;                   //闯红灯的人数
uint16_t foul_flags  = 1 ;                   //闯红灯确定标志位
uint16_t dat = 0         ;                   //接收上位机发送的数据
/****************************TIM2初始化***********************/

void TIME3_init(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseStruct.TIM_Prescaler = (72-1);             //预分频为72，每记一次数为1us    
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up; //设置计数方式为向上计数
	TIM_TimeBaseStruct.TIM_Period = 100;                   //最大计数值为100，即100us可产生一次中断
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStruct);
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
  TIM_SetCounter(TIMER_TIM,0);
}
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	                                 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //设置NCIV中断分组，2位抢占优先级，2位响应优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断通道为TIM3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //设置抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //设置响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //通道使能
	NVIC_Init(&NVIC_InitStructure);  
	
}

/**********************TIM3的中断服务函数*************************/
void TIM3_IRQHandler()
{
	static int i = 0 ; 
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET) //每产生一次中断为1s
	{
		if(i < Duty)
		{
				GPIO_SetBits(GPIOA,GPIO_Pin_0) ;
		}
		else
		{
			 GPIO_ResetBits(GPIOA,GPIO_Pin_0) ;
		}
	i++ ;
		if(i > 100)
		{
			i = 0 ;
		}
	 
			
	 TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
	}
}

/****************************USART2初始化**************/
//用于蓝牙通信
void USART2_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE) ;
	//PA2
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;                            
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//PA3
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;                     
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate = 115200;                     									     
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;   									        
  USART_InitStruct.USART_StopBits = USART_StopBits_1;        											    
  USART_InitStruct.USART_Parity = USART_Parity_No ;         											   
	 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  									    
 
	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2,ENABLE);                               
	
	
	
}

int fputc(int ch,FILE *f)
{
	USART_SendData(USART2, (u8)ch);
	while(!(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==SET));
	return ch;
}

