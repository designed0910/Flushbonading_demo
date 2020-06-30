#include "MQ2.h"
uint16_t ADC_ConvertedValue;


void MQ2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	ADC_InitTypeDef ADC_InitStructure;


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;		 

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;

	GPIO_Init(GPIOA,&GPIO_InitStructure);

 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立ADC模式

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;  //禁止扫描方式

	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//开启连续转换模式 

	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //不使用外部触发转换

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //采集数据右对齐

	ADC_InitStructure.ADC_NbrOfChannel = 1; //要转换的通道数目

	ADC_Init(ADC1, &ADC_InitStructure);

	

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//配置ADC时钟，为PCLK2的8分频，即9Hz

	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5);//配置ADC1通道11为55.5个采样周期 

	ADC_Cmd(ADC1,ENABLE);

	ADC_ResetCalibration(ADC1);//复位校准寄存器

	while(ADC_GetResetCalibrationStatus(ADC1));//等待校准寄存器复位完成

 

	ADC_StartCalibration(ADC1);//ADC校准

	while(ADC_GetCalibrationStatus(ADC1));//等待校准完成

 

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//由于没有采用外部触发，所以使用软件触发ADC转换

}

float Get_Mq2_value(void)
{
	uint8_t i = 0 ; 
	float value = 0 ;
	
	for(i=0;i<50;i++)  //获取50次，取平均值，提高精度
	{
         ADC_SoftwareStartConvCmd(ADC1,ENABLE);
         while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
         value=value+ADC_GetConversionValue(ADC1);
	}
	value = value/50 ;
	
	return value ;

}
