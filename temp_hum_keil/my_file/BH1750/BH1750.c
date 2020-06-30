
#include "BH1750.h"


/*

函数功能: IIC总线初始化

硬件连接:

        SCL-PB3

        SDA-PA15

*/

u8  BUF[3];

 

void IIC_Init(void)

{

  /*1. 开时钟*/

	RCC->APB2ENR|=1<<2; //PA

	RCC->APB2ENR|=1<<3; //PB

	RCC->APB2ENR|=1<<0; //开启AFIO时钟

	

	/*2. 配置模式*/

	AFIO->MAPR |= 0x2<<24;//关闭JTAG-DP,启用SW-DP

	GPIOA->CRH&=0x0FFFFFFF;

	GPIOA->CRH|=0x30000000;

	GPIOB->CRL&=0xFFFF0FFF;

	GPIOB->CRL|=0x00003000;

	

	/*3. 上拉*/

	GPIOA->ODR|=0x1<<15;

	GPIOB->ODR|=0x1<<3;

	

	//printf("BH1750 IIC_Init OK！ \n");

}

 

/**************************************

起始信号

**************************************/

void BH1750_Start()

{

	IIC_OUTPUT_MODE_SET(); //配置输出模式

	IIC_SDA_OUT = 1;       //拉高数据线

	IIC_SCL = 1;           //拉高时钟线

	delay_us(5);            //延时

	IIC_SDA_OUT = 0;       //产生下降沿

	delay_us(5);            //延时

	IIC_SCL = 0;           //拉低时钟线

}

 

/*

函数功能: 停止信号

*/

void BH1750_Stop()

{

	IIC_OUTPUT_MODE_SET(); 	//配置输出模式

	IIC_SDA_OUT = 0;        //拉低数据线

	IIC_SCL = 1;            //拉高时钟线

	delay_us(5);             //延时

	IIC_SDA_OUT = 1;        //产生上升沿

	delay_us(5);             //延时

}

 

/**************************************

发送应答信号

入口参数:ack (0:ACK 1:NAK)

**************************************/

void BH1750_SendACK(char ack)

{

	IIC_OUTPUT_MODE_SET(); 	  //配置输出模式

	IIC_SCL = 0;              //拉低时钟线

	if(ack) IIC_SDA_OUT = 1; 

	else IIC_SDA_OUT = 0;     //写应答信号

	delay_us(2);               //延时

	IIC_SCL = 1;              //拉高时钟线

	delay_us(5);               //延时

	IIC_SCL = 0;              //拉低时钟线

}

 

/**************************************

接收应答信号

**************************************/

char BH1750_RecvACK()

{

	u8 cnt=0;

	IIC_INPUT_MODE_SET();//输入模式

	IIC_SDA_OUT=1; 

	delay_us(1);

	IIC_SCL=1;

	delay_us(1);

	while(IIC_SDA_IN)//等待应答

	{

		cnt++;

		delay_us(1);

		if(cnt>=250)//等待时间过长，产生停止信号，返回1，表示接收应答失败

		{

			BH1750_Stop();

			//printf("N0 ACK\n");

			return 1;

		}

	}

	IIC_SCL=0;//应答成功，则SCL变低

	return 0;

}

 

/**************************************

向IIC总线发送一个字节数据

**************************************/

void BH1750_SendByte(u8 data)

{

	u8 i;

	IIC_OUTPUT_MODE_SET();

	IIC_SCL = 0;//拉低时钟线, 数据准备发送

	delay_us(2);//延时

	for (i=0;i<8;i++)//从高位开始一位一位地传送

	{

		if(data&0x80) IIC_SDA_OUT=1;//送数据口

		else IIC_SDA_OUT=0;

		data <<= 1;//移出数据的最高位

		IIC_SCL = 1; //拉高时钟线,发送数据

		delay_us(2);//延时

		IIC_SCL = 0;//拉低时钟线，数据发送完毕

		delay_us(2);//延时

	}

}

/**************************************

从IIC总线接收一个字节数据

**************************************/

u8 BH1750_RecvByte()

{

	u8 i;

	u8 data = 0;

	IIC_INPUT_MODE_SET();//使能内部上拉,准备读取数据,

	for (i=0;i<8;i++)//8位计数器

	{

		IIC_SCL=0;//置时钟线为低，准备接收数据位

		delay_us(2);//时钟低电平周期大于4.7μs

		IIC_SCL=1;//置时钟线为高使数据线上数据有效，主机开始读数据，从机不能再改变数据了，即改变SDA的电平

		data <<= 1;

		IIC_SCL = 1;//拉高时钟线

		if(IIC_SDA_IN) data|=0x01;//读数据               

		delay_us(1);//延时

	}

	IIC_SCL=0;

	return data;

}

 

void Single_Write_BH1750(u8 REG_Address)

{

	BH1750_Start();//起始信号

	BH1750_SendByte(SlaveAddress);//发送设备地址+写信号

	BH1750_RecvACK();

	BH1750_SendByte(REG_Address);//内部寄存器地址，请参考中文pdf22页 

	BH1750_RecvACK();

	BH1750_Stop();//发送停止信号

}

/*********************************************************

连续读出BH1750内部数据

*********************************************************/

void Multiple_Read_BH1750()

{   

  BH1750_Start();//起始信号

  BH1750_SendByte(SlaveAddress+1);//发送设备地址+读信号

	BH1750_RecvACK();

	BUF[0] = BH1750_RecvByte();//BUF[0]存储0x32地址中的数据

	BH1750_SendACK(0);

	BUF[1] = BH1750_RecvByte();//BUF[0]存储0x32地址中的数据

	BH1750_SendACK(1);

	BH1750_Stop();//停止信号

	delay_ms(5);

}

 

/*********************************************************

读出数据并合成实际光强值

*********************************************************/

float Get_Sunlight_Value()

{

	int dis_data=0;

	float temp;

	u8 i=0;

	Single_Write_BH1750(0x01);// power on

	Single_Write_BH1750(0x10);// H- resolution mode

	delay_ms(180);//延时180ms

	Multiple_Read_BH1750();//连续读出数据，存储在BUF中  

	for(i=0;i<3;i++)

	//printf("0X%X ",BUF[i]);

	dis_data=BUF[0];

	dis_data=(dis_data<<8)+BUF[1];//合成数据 

	temp=(float)dis_data/1.2;

//	printf("Sunlight=%0.2f lx\n",temp);
return temp ;

}
