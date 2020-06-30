#include "menue.h"
#include "stdio.h"
uint8_t Select = 1 ;                 //菜单选项
extern u8 wd;                        //温度
extern u8 sd;                        //湿度	 
unsigned char display[80] ;          //显示数组
int Duty = 10 ;                      //占空比，模拟输出PWM波，Duty为占空比，占空比越高，灯越亮
uint8_t Aim = 25 ;                    //目标温度
uint8_t Set = 25 ;                    //设定温度
uint8_t Mode = 1 ;                    //1为自动模式，2为手动模式
uint8_t Low  = 10 ;                   //最低温度
uint8_t High  = 28 ;                  //最高温度
uint8_t times  = 8 ;

void Key_Init()    
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(Key_Clock|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
 
  GPIO_InitStruct.GPIO_Pin  = Key_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU    ;
	GPIO_Init(Key_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin  = Key_2;
	GPIO_Init(Key_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin  = Key_3;
	GPIO_Init(Key_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin  = Key_4;
	GPIO_Init(Key_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP    ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	 
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 ;
	 GPIO_Init(GPIOC,&GPIO_InitStruct) ;
	
}

Key Key_Get()
{
	 if(GPIO_ReadInputDataBit(Key_Port,Key_1) == 0)
	 {
		delay_ms(QuDou_time) ;
		 {
			 if(GPIO_ReadInputDataBit(Key_Port,Key_1) == 0)
			 {
				  return KEY_1 ;
			 }
		 }
	 }
	 
	  if(GPIO_ReadInputDataBit(Key_Port,Key_2) == 0)
	 {
		delay_ms(QuDou_time) ;
		 {
			 if(GPIO_ReadInputDataBit(Key_Port,Key_2) == 0)
			 {
				  return KEY_2 ;
			 }
		 }
	 }
	 
	  if(GPIO_ReadInputDataBit(Key_Port,Key_3) == 0)
	 {
		delay_ms(QuDou_time) ;
		 {
			 if(GPIO_ReadInputDataBit(Key_Port,Key_3) == 0)
			 {
				  return KEY_3 ;
			 }
		 }
	 }
	 
	  if(GPIO_ReadInputDataBit(Key_Port,Key_4) == 0)
	 {
		delay_ms(QuDou_time) ;
		 {
			 if(GPIO_ReadInputDataBit(Key_Port,Key_4) == 0)
			 {
				  return KEY_4 ;
			 }
		 }
	 }
	 
	 return KEY_None ;
}

void Key_Function()
{
	Key K ;
	K = Key_Get() ;
	if(K != KEY_None )
	{
		 if(K == KEY_4)
		 {
			 Select-- ;
			 if(Select < 1)
			 {
				Select = 7 ;
			 }
			 LCD_Fill(0x00);  //初始清屏
		 }
		 
		if(K == KEY_1)
		 {
			 Select++ ;
			 if(Select > 7)
			 {
				 Select = 1 ;
			 }
			 LCD_Fill(0x00);  //初始清屏
		 }
		 
		 	if(K == KEY_2)
		 {
			switch(Select)
			{
				case 3 : Mode-- ; if(Mode < 1) Mode = 2 ;break ;				
				case 4 : Set-- ; break ;
				case 5 : Aim-- ; break ;
				case 6 : Low-- ; break ;
				case 7 : High-- ; break ;
			}
		 }
		 
		 if(K == KEY_3)
		 {
			switch(Select)
			{
				case 3 : Mode++ ; if(Mode > 2) Mode = 1 ; break ;
				case 4 : Set++ ; break ;
				case 5 : Aim++ ; break ;
				case 6 : Low++ ; break ;
				case 7 : High++ ; break ;
				
			}
		 }
	}
}
void Display()
{
unsigned	char xx[8] = "%" ;
	switch(Select)
	{
		case 1 : Select1() ; break ;
		case 2 : Select2() ; break ;
		case 3 : Select3() ; break ;
		case 4 : Select4() ; break ;
		case 5 : Select4() ; break ;
		case 6 : Select4() ; break ;
	  case 7 : Select4() ; break ;
	}
	
				if(Select < 5)
			{
				 sprintf((char *)display,"温度：") ;	
				 LCD_Print(12,0,display) ;
				
				 sprintf((char *)display,"%d ℃",wd) ;	
				 LCD_Print(55,0,display) ;
				
				 sprintf((char *)display,"湿度：") ;	
				 LCD_Print(12,2,display) ;
				
				 sprintf((char *)display,"%d",sd) ;	
				 LCD_Print(55,2,display) ;
				 LCD_P8x16Str(75,2,xx); 
				
				sprintf((char *)display,"MODE:") ;	
				 LCD_Print(12,4,display) ;
				if(Mode == 1)	
				{
				 sprintf((char *)display,"auto    ") ;	
				 LCD_Print(56,4,display) ;
				 Duty =Aim +  (Aim -  wd)*1.7 ;
			
				}
				else
				{
					 sprintf((char *)display,"operate") ;	
				 LCD_Print(56,4,display) ;
					 Duty = Set ;  
				}
				sprintf((char *)display,"加热器：") ;	
				 LCD_Print(12,6,display) ;
				
					sprintf((char *)display,"%d℃",Duty) ;	
				 LCD_Print(75,6,display) ;
			}else if(Select== 5)
			{
				 sprintf((char *)display,"目标温度：") ;	
				 LCD_Print(12,6,display) ;
				
				 sprintf((char *)display,"%d℃",Aim) ;	
				 LCD_Print(80,6,display) ;
				
				 sprintf((char *)display,"湿度：") ;	
				 LCD_Print(12,0,display) ;
				
				 sprintf((char *)display,"%d",sd) ;	
				 LCD_Print(55,0,display) ;
				 LCD_P8x16Str(75,0,xx); 
				
				sprintf((char *)display,"MODE:") ;	
				 LCD_Print(12,2,display) ;
				if(Mode == 1)	
				{
				 sprintf((char *)display,"auto    ") ;	
				 LCD_Print(56,2,display) ;
					Duty =Aim +  (Aim -  wd)*1.7 ;
				}
				else
				{
					 sprintf((char *)display,"operate") ;	
				   LCD_Print(56,2,display) ;
					 Duty = Set ;  
				}
				sprintf((char *)display,"加热器：") ;	
				 LCD_Print(12,4,display) ;
				
					sprintf((char *)display,"%d℃",Duty) ;	
				 LCD_Print(75,4,display) ;
			} else if(Select == 6)
			{
				 sprintf((char *)display,"目标温度：") ;	
				 LCD_Print(12,4,display) ;
				
				 sprintf((char *)display,"%d℃",Aim) ;	
				 LCD_Print(80,4,display) ;
				
				 sprintf((char *)display,"最低温度：") ;	
				 LCD_Print(12,6,display) ;
				
				 sprintf((char *)display,"%d℃",Low) ;	
				 LCD_Print(80,6,display) ;
				
				
				sprintf((char *)display,"MODE:") ;	
				 LCD_Print(12,0,display) ;
				if(Mode == 1)	
				{
				 sprintf((char *)display,"auto    ") ;	
				 LCD_Print(56,0,display) ;
				Duty =Aim +  (Aim -  wd)*1.7 ;
				}
				else
				{
					 sprintf((char *)display,"operate") ;	
				   LCD_Print(56,0,display) ;
					 Duty = Set ;  
				}
				sprintf((char *)display,"加热器：") ;	
				 LCD_Print(12,2,display) ;
				
					sprintf((char *)display,"%d℃",Duty) ;	
				 LCD_Print(75,2,display) ;
			}else if(Select == 7)
			{
				 sprintf((char *)display,"目标温度：") ;	
				 LCD_Print(12,2,display) ;
				
				 sprintf((char *)display,"%d℃",Aim) ;	
				 LCD_Print(80,2,display) ;
				
				 sprintf((char *)display,"最低温度：") ;	
				 LCD_Print(12,4,display) ;
				
				 sprintf((char *)display,"%d℃",Low) ;	
				 LCD_Print(80,4,display) ;
				
				 sprintf((char *)display,"最高温度：") ;	
				 LCD_Print(12,6,display) ;
				
				 sprintf((char *)display,"%d℃",High) ;	
				 LCD_Print(80,6,display) ;
				
				
				
				sprintf((char *)display,"加热器：") ;	
				 LCD_Print(12,0,display) ;
				
					sprintf((char *)display,"%d℃",Duty) ;	
				 LCD_Print(75,0,display) ;
			}
			
}

void Select1() 
{
	 sprintf((char *)display,">") ;	
	 LCD_Print(0,0,display) ;
	
	 sprintf((char *)display," ") ;	
	 LCD_Print(0,2,display) ;
	
	sprintf((char *)display," ") ;	
	 LCD_Print(0,4,display) ;
	
	sprintf((char *)display," ") ;	
	 LCD_Print(0,6,display) ;
	
}

void Select2() 
{
	 sprintf((char *)display," ") ;	
	 LCD_Print(0,0,display) ;
	
	 sprintf((char *)display,">") ;	
	 LCD_Print(0,2,display) ;
	
	sprintf((char *)display," ") ;	
	 LCD_Print(0,4,display) ;
	
	sprintf((char *)display," ") ;	
	 LCD_Print(0,6,display) ;
	
}

void Select3() 
{
	 sprintf((char *)display," ") ;	
	 LCD_Print(0,0,display) ;
	
	 sprintf((char *)display," ") ;	
	 LCD_Print(0,2,display) ;
	
	sprintf((char *)display,">") ;	
	 LCD_Print(0,4,display) ;
	
	sprintf((char *)display," ") ;	
	 LCD_Print(0,6,display) ;
	
}

void Select4() 
{
	 sprintf((char *)display," ") ;	
	 LCD_Print(0,0,display) ;
	
	 sprintf((char *)display," ") ;	
	 LCD_Print(0,2,display) ;
	
	sprintf((char *)display," ") ;	
	 LCD_Print(0,4,display) ;
	
	sprintf((char *)display,">") ;	
	 LCD_Print(0,6,display) ;
	
}

void Bee()
{

}








