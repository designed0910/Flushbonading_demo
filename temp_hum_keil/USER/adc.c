#include "adc.h"

void Adc_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
	
	RCC->APB2RSTR |= 0x1<<9;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); 	//72/6=12 ��󲻳���14MHz

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		//����ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);		//ͨ��1 �������˳��ֵ1 ����ʱ��239.5����
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);						//������λУ׼
	delay_ms(300); //while(ADC_GetResetCalibrationStatus(ADC1));		//�ȴ���λ����
	
//	LCD1602_ShowStr(0,0,"hello, world!");
	ADC_StartCalibration(ADC1);						//����ADУ׼
	delay_ms(300); //while(ADC_GetCalibrationStatus(ADC1));			//�ȴ�У׼����
}

u16 Get_Adc(void)
{
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));			//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);					//���ز���ֵ
}























