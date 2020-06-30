#include "stm32f10x.h"
#include "LQ12864.h"
#include "adc.h"
#include <stdio.h>
void main_delay(u32 i)
{
	while(i--);
}


void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 ;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);	
};
void key_GPIO_Config()
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
};

void led_1()
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3)==1)
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	}
	else
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	}
};
void led_2()
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_4)==1)
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	else
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
};
void fan()
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}	
	else
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
};
void buzzer()
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==1)
	{
  GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
	else
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}
};
int read()
{
	int key=4;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1)
		key=0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)
		key=1;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)==1)
		key=2;
	return key;
};
int main(void)
{	 
	u8 temp;
	uint16_t val;
	u8 hum;
	int key,i=3;
	char buf_temp[30] = { 0 };
	char buf_hum[30] = { 0 };
	char buf_val[100] = { 0 };
	delay_init();
	LCD_Init();
	Adc_Init();
	DHT11_Init();	
	LED_GPIO_Config();
	key_GPIO_Config();
	while(1)
	{
		DHT11_Read_Data(&temp, &hum);
		main_delay(10000);
		val = Get_Adc();  
		sprintf(buf_temp, "Temp: %d", temp);
		sprintf(buf_hum, "Hum: %d", hum);
		sprintf(buf_val, "Ligth:%d   ", val);
		LCD_P6x8Str(0, 0, (unsigned char *)buf_temp);
		LCD_P6x8Str(0, 1, (unsigned char *)buf_hum);
		LCD_P6x8Str(0, 2, (unsigned char *)buf_val);
  	LCD_P6x8Str(0, 3, (unsigned char *)"light_1");	
		LCD_P6x8Str(0, 4, (unsigned char *)"light_2");	
		LCD_P6x8Str(0, 5, (unsigned char *)"fan_0");	
		LCD_P6x8Str(0, 6, (unsigned char *)"buzz_0");	
		LCD_P6x8Str(93,i,(unsigned char *)"*");
		if(temp >28)  
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
				
		}
		else
		{			
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
		}
		key=read();
		main_delay(10000);
			if(key==4)  
				continue;
			if(key==0)    
			{
				LCD_P6x8Str(93,i,(unsigned char *)" ");
				i=i-1;
				if(i<3)
				i=6;
			main_delay(100000);
			}
			if(key==1)  
			{
				LCD_P6x8Str(93,i,(unsigned char *)" ");
				i=i+1;
				if(i>6)
					i=3;
			main_delay(100000);
			}
			if(key==2)   
			{
			switch(i)
			{
				case 3:
					led_1();
				break;
				case 4:
					led_2();
				break;
				case 5:
					fan();
				break;
				case 6:
					buzzer();
				break;
			}
			main_delay(100000);
			}
	}
}