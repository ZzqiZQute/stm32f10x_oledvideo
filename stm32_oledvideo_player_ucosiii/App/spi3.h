#ifndef __SPI3_H
#define __SPI3_H
#include "sys.h"
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

 
 				  	    													  
void SPI3_Init(void);			 //��ʼ��SPI��
void SPI3_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
void SPI3_WriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

