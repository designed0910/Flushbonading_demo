#include "adc.h"

void Adc_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
	
	RCC->APB2RSTR |= 0x1<<9;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); 	//72/6=12 最大不超过14MHz

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		//连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);		//通道1 规则采样顺序值1 采样时间239.5周期
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);						//开启复位校准
	delay_ms(300); //while(ADC_GetResetCalibrationStatus(ADC1));		//等待复位结束
	
//	LCD1602_ShowStr(0,0,"hello, world!");
	ADC_StartCalibration(ADC1);						//开启AD校准
	delay_ms(300); //while(ADC_GetCalibrationStatus(ADC1));			//等待校准结束
}

u16 Get_Adc(void)
{
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));			//等待转换结束
	return ADC_GetConversionValue(ADC1);					//返回采样值
}























