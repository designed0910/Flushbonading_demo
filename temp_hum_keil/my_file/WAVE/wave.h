#ifndef _WAVE_H
#define _WAVE_H

//TRIG---PA2 	ECHO---PA11
  #include "stm32f10x_conf.h"
extern float Distance;

#define Trig GPIO_Pin_15

#define Echo GPIO_Pin_4

#define LED_RED      GPIO_Pin_11
#define Bee          GPIO_Pin_10

extern float Distance ;

void Wave_SRD_Init(void);        

void Wave_SRD_Strat(void);

void delay_us(unsigned int time) ;

void LED_init(void) ;

void Bee_contral(void) ;

void hcsr04_NVIC(void) ;

void Hcsr04Init() ;

//u32 GetEchoTimer(void) ;

float Hcsr04GetLength(void ) ;


#endif

