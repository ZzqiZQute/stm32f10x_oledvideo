#include "spi3.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/13 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


SPI_InitTypeDef  SPI_InitStructure3;

void SPI3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5);
	GPIO_SetBits(GPIOC,GPIO_Pin_7|GPIO_Pin_8);
  // 36M
	
	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure3.SPI_Direction = SPI_Direction_1Line_Tx;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure3.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure3.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure3.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure3.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure3.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure3.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure3.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure3.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI3, &SPI_InitStructure3);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����
	
	//SPI3_WriteByte(0xff);//��������		 
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
  
void SPI3_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure3.SPI_BaudRatePrescaler = SpeedSet ;
  SPI_Init(SPI3, &SPI_InitStructure3);
	SPI_Cmd(SPI3,ENABLE);
} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
void SPI3_WriteByte(u8 TxData)
{		
			 	
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  
	SPI_I2S_SendData(SPI3, TxData); //ͨ������SPIx����һ������
				    
}

