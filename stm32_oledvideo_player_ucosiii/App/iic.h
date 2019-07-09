#ifndef __IIC_H
#define __IIC_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
#define IIC_MODE 0
   						  
//-----------------IIC IIC端口定义----------------  					   

#define IIC_SCLK_Clr() (GPIOB->BRR = GPIO_Pin_6)
#define IIC_SCLK_Set() (GPIOB->BSRR = GPIO_Pin_6)

#define IIC_SDIN_Clr() (GPIOB->BRR = GPIO_Pin_7)
#define IIC_SDIN_Set() (GPIOB->BSRR = GPIO_Pin_7)

 		     
#define IIC_CMD  0	//写命令
#define IIC_DATA 1	//写数据
   							   		    
void IIC_Init(void);
void Delay_50ms(unsigned int Del_50ms);

void Delay_for_IIC(void);
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
uint8_t IIC_Write_One_Byte(uint8_t DevAddr,uint8_t RegAddr,uint8_t Data);
void IIC_Wait_Ack(void);

#endif  
