#ifndef __OLED_IIC_H
#define __OLED_IIC_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() (GPIOB->BRR = GPIO_Pin_0)
#define OLED_SCLK_Set() (GPIOB->BSRR = GPIO_Pin_0)

#define OLED_SDIN_Clr() (GPIOB->BRR = GPIO_Pin_1)
#define OLED_SDIN_Set() (GPIOB->BSRR = GPIO_Pin_1)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数

void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);


void OLED_Refresh_Gram(void);
void OLED_Set_Pos(unsigned char  x, unsigned char  y);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode);
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char  no);	  
void OLED_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size);
void OLED_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p);
void OLED_Clear(void);
void OLED_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot);  
void OLED_MarkDirty(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2);
void OLED_DrawLine(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2);
void OLED_DrawRect(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2);
void OLED_DrawEllipse(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2);
void OLED_DrawFullImage(unsigned char* buf);
void OLED_DrawDifference(unsigned char* buf1,unsigned char* buf2);
#endif  
