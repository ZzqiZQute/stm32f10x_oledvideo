#include "oled_spi.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "string.h"
#include "math.h"
#include "spi3.h"
#include <stdio.h>
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 	
unsigned char  OLED_GRAM[128][8];	
unsigned char  MODIFIED[128][8];
unsigned char  PAGEMODIFIED[8];

/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
void OLED_Refresh_Gram(void)
{
	unsigned char  i,n;		    
	for(i=0;i<8;i++)  
	{  
		if(PAGEMODIFIED[i]){
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
	//	OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
	//	OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
		if(MODIFIED[n][i])
			{
				//OLED_WR_Byte(0xb0+n,OLED_CMD);
		    OLED_WR_Byte(((n&0xf0)>>4)|0x10,OLED_CMD);
		    OLED_WR_Byte((n&0x0f),OLED_CMD); 
				OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
			}		
		}			
	}   
	memset(MODIFIED,0,128*8);
	memset(PAGEMODIFIED,0,8);
}
//void IIC_Start2()
//{

//	OLED_SCLK_Set() ;
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	OLED_SDIN_Set();
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	OLED_SDIN_Clr();
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	OLED_SCLK_Clr();
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//}

///**********************************************
//IIC Stop
//**********************************************/
//void IIC_Stop2()
//{
//  OLED_SCLK_Set() ;
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//  OLED_SCLK_Clr();
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	OLED_SDIN_Clr();
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	OLED_SDIN_Set();
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	
//}

//void IIC_Wait_Ack2()
//{

//	//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
//	//GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
//	//OLED_SCL = 1;
//	//delay_us(50000);
///*	while(1)
//	{
//		if(!OLED_SDA)				//判断是否接收到OLED 应答信号
//		{
//			//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
//			//GPIOB->CRH |= 0x00030000;
//			return;
//		}
//	}
//*/
//	OLED_SCLK_Set() ;
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	OLED_SCLK_Clr();
//	__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//}
/**********************************************
// IIC Write byte
**********************************************/

//void Write_IIC_Byte2(unsigned char IIC_Byte)
//{
//	unsigned char i;
//	unsigned char m,da;
//	da=IIC_Byte;

//		OLED_SCLK_Clr();
//		__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	for(i=0;i<8;i++)		
//	{
//		m=da;
//		m=m&0x80;
//		if(m==0x80)
//		{OLED_SDIN_Set();}
//		else OLED_SDIN_Clr();
//			__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//			da=da<<1;
//			OLED_SCLK_Set();
//			__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//		OLED_SCLK_Clr();
//			__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");__ASM volatile("nop");
//	}


//}
/**********************************************
// IIC Write Command
**********************************************/
//void Write_IIC_Command2(unsigned char IIC_Command)
//{
//   IIC_Start2();
//   Write_IIC_Byte2(0x78);            //Slave address,SA0=0
//	IIC_Wait_Ack2();	
//   Write_IIC_Byte2(0x00);			//write command
//	IIC_Wait_Ack2();	
//   Write_IIC_Byte2(IIC_Command); 
//	IIC_Wait_Ack2();	
//   IIC_Stop2();
////	u8 byte[3]={0x78,0x00};
////	byte[2]=IIC_Command;
////	I2C_Master_BufferWrite(byte,3);
//	
//	
//}
/**********************************************
// IIC Write Data
**********************************************/
//void Write_IIC_Data2(unsigned char IIC_Data)
//{
//   IIC_Start2();
//   Write_IIC_Byte2(0x78);			//D/C#=0; R/W#=0
//	IIC_Wait_Ack2();	
//   Write_IIC_Byte2(0x40);			//write data
//	IIC_Wait_Ack2();	
//   Write_IIC_Byte2(IIC_Data);
//	IIC_Wait_Ack2();	
//   IIC_Stop2();
////	u8 byte[3]={0x78,0x40};
////	byte[2]=IIC_Data;
////	I2C_Master_BufferWrite(byte,3);
//}
void Write_SPI_Command2(unsigned char SPI_Command)
{
	OLED_DC_Clr();
	OLED_CS_Clr();
	SPI3_WriteByte(SPI_Command);

}
void Write_SPI_Data2(unsigned char SPI_Data)
{
	OLED_DC_Set();
	OLED_CS_Clr();
	SPI3_WriteByte(SPI_Data);
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_SPI_Data2(dat);
   
		}
	else {
   Write_SPI_Command2(dat);
		
	}
}

/***********************Delay****************************************/

  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

//设置坐标点
void OLED_Set_Pos(unsigned char  x, unsigned char  y) 
{ 
		OLED_WR_Byte(0xb0+y/8,OLED_CMD);
		OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte((x&0x0f),OLED_CMD); 
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t)
{
	unsigned char  pos,bx,temp=0;
		//OLED_Set_Pos(x, y);  
		if(x>127||y>64)return;//超出范围了.
		pos=y/8;
		bx=y%8;
		temp=1<<bx;
		if(t)OLED_GRAM[x][pos]|=temp;
		else OLED_GRAM[x][pos]&=~temp;	
	  
     //设置显示位置—列高地址  
		PAGEMODIFIED[pos]=1; 	
	  MODIFIED[x][pos]=1; 
	
  	//OLED_WR_Byte(OLED_GRAM[x][pos],OLED_DATA); 
	  

}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode)
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char  no)//显示中文
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	unsigned char  mode=1;
	//chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<32;t++)
    {   
	
			temp= CHINESE[no][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
      }          
}  
	  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size)
{         	
	unsigned char  t,temp;
	unsigned char  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void OLED_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p)
{
#define MAX_CHAR_POSX 127
#define MAX_CHAR_POSY 63         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   


//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	memset(OLED_GRAM,0,128*8);
	memset(MODIFIED,1,128*8);
	memset(PAGEMODIFIED,1,8);
	OLED_Refresh_Gram();//更新显示
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot)  
{  
	unsigned char  x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}


void OLED_MarkDirty(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2)  
{  
	signed char i;
	char j;
	char top=y1/8;
	char bottom=y2/8;
	for(i=top-1;i<=bottom+1;i++){
		if(i>=0&&i<8)
			PAGEMODIFIED[i]=1;
		else if(i==0)
			PAGEMODIFIED[7]=1;
		else if(i==8)
			PAGEMODIFIED[0]=1;
		for(j=x1;j<=x2;j++){
			MODIFIED[j][i]=1;
			OLED_GRAM[j][i]=0;
		}
	}
}
void OLED_DrawLine(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2){
	u8 i;
	for(i=x1;i<=x2;i++){
		OLED_DrawPoint(i,y1,1);
		OLED_DrawPoint(i,y2,1);
	}
	for(i=y1;i<=y2;i++){
		OLED_DrawPoint(x1,i,1);
		OLED_DrawPoint(x2,i,1);
	}
}
void OLED_DrawRect(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2){
	OLED_DrawLine(x1,y1,x2,y1);
	OLED_DrawLine(x1,y2,x2,y2);
	OLED_DrawLine(x1,y1,x1,y2);
	OLED_DrawLine(x2,y1,x2,y2);
}
void OLED_DrawEllipse(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2){

	int i;
	int x,y,v;
	int a=(x2-x1)/2;
	int b=(y2-y1)/2;
	int centerx=((int)x1+x2)>>1;
	int centery=((int)y1+y2)>>1;
	int lastv=-1;
	int tmp=0;
	int tmp2=0;

	
	for(i=x1-centerx;i<=x2-centerx;i++){
		x=centerx+i;
		tmp=0;
		v=b*sqrt(1-(float)i*i/(a*a));
		if(lastv!=-1&&abs(v-lastv)>1){
			
			tmp=(abs(v-lastv))*(v-lastv>0?-1:1);
		
		}
		y=centery+v;
		OLED_DrawPoint(x,y,1);
		y=centery-v;
		OLED_DrawPoint(x,y,1);
		for(tmp2=0;tmp2<abs(tmp);tmp2++){
		y=centery+v-tmp2;
		OLED_DrawPoint(x,y,1);
		y=centery-v+tmp2;
		OLED_DrawPoint(x,y,1);
		}
		lastv=v;
			
	}
		
}
void OLED_DrawFullImage(unsigned char* buf){
			unsigned char  i,n;		
	memcpy(OLED_GRAM,buf,128*8);
    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
	//	OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
	//	OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)

			{
				//OLED_WR_Byte(0xb0+n,OLED_CMD);
		    OLED_WR_Byte(((n&0xf0)>>4)|0x10,OLED_CMD);
		    OLED_WR_Byte((n&0x0f),OLED_CMD); 
				OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
			}			
	}   
	
}
void LCD_WrCmd(u8 cmd){
	OLED_WR_Byte(cmd,OLED_CMD);
}
//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能A端口时钟
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
// 	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);	

	SPI3_Init();
	//SPI3_SetSpeed(SPI_BaudRatePrescaler_256);
	OLED_RST_Clr();
	Delay(10000);
	OLED_RST_Set();
//	Delay(5000);
//	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
//	Delay(1000);
//	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
//	Delay(1000);
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	Delay(1000);
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
//	Delay(1000);
//	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
//	Delay(1000);
//	OLED_WR_Byte(0x81,OLED_CMD); // contract control
//	Delay(1000);
//	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
//	Delay(1000);
//	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
//	Delay(1000);
//	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
//	Delay(1000);
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	Delay(1000);	
//	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
//	Delay(1000);
//	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
//	Delay(1000);
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
//	Delay(1000);
//	OLED_WR_Byte(0x00,OLED_CMD);//
//	Delay(1000);
//	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
//	Delay(1000);
//	OLED_WR_Byte(0x80,OLED_CMD);//
//	Delay(1000);
//	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
//	Delay(1000);
//	OLED_WR_Byte(0x05,OLED_CMD);//
//	Delay(1000);
//	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
//	Delay(1000);
//	OLED_WR_Byte(0xF1,OLED_CMD);//
//	Delay(1000);
//	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
//	Delay(1000);
//	OLED_WR_Byte(0x12,OLED_CMD);//
//	Delay(1000);
//	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
//	Delay(1000);
//	OLED_WR_Byte(0x30,OLED_CMD);//
//	Delay(1000);
//	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
//	Delay(1000);
//	OLED_WR_Byte(0x14,OLED_CMD);//
//	Delay(1000);
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

  LCD_WrCmd(0xae);//--turn off oled panel
  LCD_WrCmd(0x00);//---set low column address
  LCD_WrCmd(0x10);//---set high column address
  LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  LCD_WrCmd(0x81);//--set contrast control register
  LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
  LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0???? 0xa1??
  LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0???? 0xc8??
  LCD_WrCmd(0xa6);//--set normal display
  LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  LCD_WrCmd(0x3f);//--1/64 duty
  LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);//-not offset
  LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  LCD_WrCmd(0xd9);//--set pre-charge period
  LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  LCD_WrCmd(0xda);//--set com pins hardware configuration
  LCD_WrCmd(0x12);
  LCD_WrCmd(0xdb);//--set vcomh
  LCD_WrCmd(0x40);//Set VCOM Deselect Level
  LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x02);//
  LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);//--set(0x10) disable
  LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  LCD_WrCmd(0xaf);//--turn on oled panel
	LCD_WrCmd(0xaf);
}  

void OLED_DrawDifference(unsigned char* buf1,unsigned char* buf2){
	int i,j;

	for(i=0;i<128;i++){
		for(j=0;j<8;j++){
			if(buf1[8*i+j]!=buf2[8*i+j]){
				MODIFIED[i][j]=1;
        PAGEMODIFIED[j]=1;
				
		}
	}
}
	memcpy(OLED_GRAM,buf1,1024);
	OLED_Refresh_Gram();
	
}

