#ifndef _DTH11_H_
#define _DTH11_H_

   #include "stm32f10x.h"

#define IO_DHT11           GPIO_Pin_15 //引入中间变量，方便移植

#define GPIO_DHT11         GPIOC    //引入中间变量，方便移植

 

#define DHT11_DQ_High    GPIO_SetBits(GPIO_DHT11,IO_DHT11) 

#define DHT11_DQ_Low     GPIO_ResetBits(GPIO_DHT11,IO_DHT11)

 
void delay(int32_t us)  ;

void DHT11_IO_OUT(void);//温湿度模块输出函数

void DHT11_IO_IN(void); //温湿度模块输入函数

void DHT11_Init(void);  //初始化DHT11

u8   DHT11_Read_Data(u8 *temp,u8 *humi); //读取温湿度

u8   DHT11_Read_Byte(void);              //读出一个字节

u8   DHT11_Read_Bit(void);               //读出一个位

u8   DHT11_Check(void);                  //检测是否存在DHT11

void DHT11_Rst(void);                    //复位DHT11    
uint8_t DHT_ByteRead(unsigned char *dat) ;
#endif

