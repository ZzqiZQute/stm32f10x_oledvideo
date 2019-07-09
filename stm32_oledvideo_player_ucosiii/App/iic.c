#include "iic.h"
#include "stdlib.h"
#include "delay.h"
#include "string.h"
#include "math.h"
#include <stdio.h>

/**********************************************
//IIC Start
**********************************************/
void IIC_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //??A????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //???GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	
}
void IIC_Start()
{

	IIC_SCLK_Set();
	IIC_SDIN_Set();
	IIC_SDIN_Clr();
	IIC_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
  IIC_SCLK_Set() ;
//	IIC_SCLK_Clr();
	IIC_SDIN_Clr();
	IIC_SDIN_Set();
	
}
void IIC_Wait_Ack()
{

	//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
	//GPIOB->CRH |= 0x00080000;
//	IIC_SDA = 1;
//	delay_us(1);
	//IIC_SCL = 1;
	//delay_us(50000);
/*	while(1)
	{
		if(!IIC_SDA)				//判断是否接收到IIC 应答信号
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}
*/
	IIC_SCLK_Set() ;
	IIC_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	IIC_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	IIC_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{IIC_SDIN_Set();}
		else IIC_SDIN_Clr();
			da=da<<1;
		IIC_SCLK_Set();
		IIC_SCLK_Clr();
		}


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
//	u8 byte[3]={0x78,0x00};
//	byte[2]=IIC_Command;
//	I2C_Master_BufferWrite(byte,3);
	
	
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
//	u8 byte[3]={0x78,0x40};
//	byte[2]=IIC_Data;
//	I2C_Master_BufferWrite(byte,3);
}
void IIC_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}
}

/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}


void Delay_for_IIC(void)
{
}
uint8_t IIC_Write_One_Byte(uint8_t DevAddr,uint8_t RegAddr,uint8_t Data)
{
	IIC_Start();
   Write_IIC_Byte(DevAddr);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(RegAddr);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(Data); 
	IIC_Wait_Ack();	
   IIC_Stop();
	return 0;
}
