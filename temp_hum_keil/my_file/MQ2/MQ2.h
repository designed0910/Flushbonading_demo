#ifndef _MQ2_H_
#define _MQ2_H_
     #include "stm32f10x.h"

#define ADC1_DR_Address ((uint32_t)0x40012400+0x4C)


void MQ2_Init(void) ;
float Get_Mq2_value(void) ;

#endif