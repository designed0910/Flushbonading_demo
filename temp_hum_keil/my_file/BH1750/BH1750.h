#ifndef _BH1750_H
#define _BH1750_H

#include "stm32f10x.h"

#include "sys.h"

#include "delay.h"

 

//IIC总线地址接口定义

#define IIC_SCL PBout(3)

#define IIC_SDA_OUT PAout(15)

#define IIC_SDA_IN PAin(15)

 

#define IIC_INPUT_MODE_SET()  {GPIOA->CRH&=0x0FFFFFFF;GPIOA->CRH|=0x80000000;}

#define IIC_OUTPUT_MODE_SET() {GPIOA->CRH&=0x0FFFFFFF;GPIOA->CRH|=0x30000000;}

 

#define  IIC_WRITE      0     // 写

#define  IIC_READ       1     // 读

#define	 SlaveAddress   0x46  //BH1750地址

extern u8  BUF[3];

//函数声明

void IIC_Init(void);

void BH1750_Start();

void BH1750_Stop();

void BH1750_SendACK(char ack);

char BH1750_RecvACK();

void BH1750_SendByte(u8 data);

void Single_Write_BH1750(u8 REG_Address);

void Multiple_Read_BH1750();

float Get_Sunlight_Value() ;

u8 BH1750_RecvByte();

 

#endif

