/* WM8978ģ�黶ӭ��https://shop36329318.taobao.com/����
�������Ȩ����Ϊhttps://shop36329318.taobao.com/�������޸ġ��ַ������뱣��������Ϣ*/
#include "wm8978.h"

static uint16_t WM8978_REGVAL[58]=
{
	0X0000,0X0000,0X0000,0X0000,0X0050,0X0000,0X0140,0X0000,
	0X0000,0X0000,0X0000,0X00FF,0X00FF,0X0000,0X0100,0X00FF,
	0X00FF,0X0000,0X012C,0X002C,0X002C,0X002C,0X002C,0X0000,
	0X0032,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
	0X0038,0X000B,0X0032,0X0000,0X0008,0X000C,0X0093,0X00E9,
	0X0000,0X0000,0X0000,0X0000,0X0003,0X0010,0X0010,0X0100,
	0X0100,0X0002,0X0001,0X0001,0X0039,0X0039,0X0039,0X0039,
	0X0001,0X0001
}; 

uint8_t WM8978_Write_Reg(uint8_t reg,uint16_t val)
{
	uint8_t res;
	uint8_t RegAddr;
	uint8_t RegValue;
	RegAddr=(reg<<1)|((uint8_t)((val>>8)&0x01));
	RegValue=(uint8_t)val;
	res=IIC_Write_One_Byte(WM8978_ADDR,RegAddr,RegValue);
	if(res==0)
	WM8978_REGVAL[reg]=val;
	return res;
}

uint16_t WM8978_Read_Reg(uint8_t reg)
{  
	return WM8978_REGVAL[reg];	
} 

uint8_t WM8978_Init(void)
{
	//I2C���߱����ڵ��øú���ǰ��ɳ�ʼ��
	uint8_t Res;
	Res=WM8978_Write_Reg(0,0);							//��λWM8978
	if(Res)
		return 1;															//����ָ��ʧ��,WM8978�쳣 ;; ����PLL
	WM8978_Write_Reg(1,0X1B);								//R1,MICEN����Ϊ1(MICʹ��),BIASEN����Ϊ1(ģ��������),VMIDSEL[1:0]����Ϊ:11(5K)
	WM8978_Write_Reg(2,0X1B0);							//R2,ROUT1,LOUT1���ʹ��(�������Թ���),BOOSTENR,BOOSTENLʹ��
	WM8978_Write_Reg(3,0X6C);								//R3,LOUT2,ROUT2���ʹ��(���ȹ���),RMIX,LMIXʹ��	
	WM8978_Write_Reg(6,0);									//R6,MCLK���ⲿ�ṩ
	WM8978_Write_Reg(43,1<<4);							//R43,INVROUT2����,��������
	WM8978_Write_Reg(47,1<<8);							//R47����,PGABOOSTL,��ͨ��MIC���20������
	WM8978_Write_Reg(48,1<<8);							//R48����,PGABOOSTR,��ͨ��MIC���20������
	WM8978_Write_Reg(49,1<<1);							//R49,TSDEN,�������ȱ��� 
	WM8978_Write_Reg(10,1<<3);							//R10,SOFTMUTE�ر�,128x����,���SNR 
	WM8978_Write_Reg(14,1<<3);							//R14,ADC 128x������
	
	WM8978_I2S_Cfg(2,0);										//����I2S�ӿ�ģʽ������λ������Ҫ���ã����Ŵ��豸��ʹ��
	return 0;
}
uint8_t WM8978_DeInit(void)
{
	//I2C���߱����ڵ��øú���ǰ��ɳ�ʼ��
	uint8_t Res;
	Res=WM8978_Write_Reg(0,0);							//��λWM8978
	if(Res)
		return 1;															//����ָ��ʧ��,WM8978�쳣 ;; ����PLL

	return 0;
}
uint8_t WM8978_Init_Record(void)
{
	//I2C���߱����ڵ��øú���ǰ��ɳ�ʼ��
	uint8_t Res;
	Res=WM8978_Write_Reg(0,0);							//��λWM8978
	if(Res)
		return 1;															//����ָ��ʧ��,WM8978�쳣 ;; ����PLL
	WM8978_Write_Reg(1,0X1B);								//R1,MICEN����Ϊ1(MICʹ��),BIASEN����Ϊ1(ģ��������),VMIDSEL[1:0]����Ϊ:11(5K)
	WM8978_Write_Reg(2,0X1B0);							//R2,ROUT1,LOUT1���ʹ��(�������Թ���),BOOSTENR,BOOSTENLʹ��
	WM8978_Write_Reg(3,0X6C);								//R3,LOUT2,ROUT2���ʹ��(���ȹ���),RMIX,LMIXʹ��	
	WM8978_Write_Reg(6,0);									//R6,MCLK���ⲿ�ṩ
	WM8978_Write_Reg(43,1<<4);							//R43,INVROUT2����,��������
	WM8978_Write_Reg(47,1<<8);							//R47����,PGABOOSTL,��ͨ��MIC���20������
	WM8978_Write_Reg(48,1<<8);							//R48����,PGABOOSTR,��ͨ��MIC���20������
	WM8978_Write_Reg(49,1<<1);							//R49,TSDEN,�������ȱ��� 
	WM8978_Write_Reg(10,1<<3);							//R10,SOFTMUTE�ر�,128x����,���SNR 
	WM8978_Write_Reg(14,1<<3);							//R14,ADC 128x������
  
	
	WM8978_Write_Reg(35,0X0f);	
	
	
	WM8978_I2S_Cfg(2,0);										//����I2S�ӿ�ģʽ������λ������Ҫ���ã����Ŵ��豸��ʹ��
	//��������
//	WM8978_ADDA_Cfg(1,0);										//����DAC
//	WM8978_Input_Cfg(0,0,0);								//�ر�����ͨ��
//	WM8978_Output_Cfg(1,0);									//����DAC���  
//¼������
//	WM8978_ADDA_Cfg(0,1);										//����ADC
//	WM8978_Input_Cfg(1,1,0);								//��������ͨ��(MIC&LINE IN)
//	WM8978_Output_Cfg(0,1);									//����BYPASS��� 
//	WM8978_MIC_Gain(46);										//MIC�������� 
//	WM8978_HPvol_Set(15,15);
//	WM8978_SPKvol_Set(0);//50

	
	return 0;
}
//WM8978 DAC/ADC����
//adcen:adcʹ��(1)/�ر�(0)
//dacen:dacʹ��(1)/�ر�(0)
void WM8978_ADDA_Cfg(uint8_t dacen,uint8_t adcen)
{
	uint16_t regval;
	regval=WM8978_Read_Reg(3);							//��ȡR3
	if(dacen)
		regval|=3<<0;													//R3���2��λ����Ϊ1,����DACR&DACL
	else 
		regval&=~(3<<0);											//R3���2��λ����,�ر�DACR&DACL.
	WM8978_Write_Reg(3,regval);							//����R3
	regval=WM8978_Read_Reg(2);							//��ȡR2
	if(adcen)
		regval|=3<<0;			        						//R2���2��λ����Ϊ1,����ADCR&ADCL
	else 
		regval&=~(3<<0);				  						//R2���2��λ����,�ر�ADCR&ADCL.
	WM8978_Write_Reg(2,regval);							//����R2	
}
//WM8978 ����ͨ������ 
//micen:MIC����(1)/�ر�(0)
//lineinen:Line In����(1)/�ر�(0)
//auxen:aux����(1)/�ر�(0) 
void WM8978_Input_Cfg(uint8_t micen,uint8_t lineinen,uint8_t auxen)
{
	uint16_t regval;  
	regval=WM8978_Read_Reg(2);							//��ȡR2
	if(micen)
		regval|=3<<2;													//����INPPGAENR,INPPGAENL(MIC��PGA�Ŵ�)
	else 
		regval&=~(3<<2);											//�ر�INPPGAENR,INPPGAENL.
 	WM8978_Write_Reg(2,regval);							//����R2 
	regval=WM8978_Read_Reg(44);							//��ȡR44
	if(micen)
		regval|=3<<4|3<<0;										//����LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA.
	else 
		regval&=~(3<<4|3<<0);									//�ر�LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA.
	WM8978_Write_Reg(44,regval);						//����R44
	if(lineinen)
		WM8978_LINEIN_Gain(5);								//LINE IN 0dB����
	else 
		WM8978_LINEIN_Gain(0);								//�ر�LINE IN
	if(auxen)
		WM8978_AUX_Gain(7);										//AUX 6dB����
	else 
		WM8978_AUX_Gain(0);										//�ر�AUX����  
}
//WM8978 MIC��������(������BOOST��20dB,MIC-->ADC���벿�ֵ�����)
//gain:0~63,��Ӧ-12dB~35.25dB,0.75dB/Step
void WM8978_MIC_Gain(uint8_t gain)
{
	gain&=0X3F;
	WM8978_Write_Reg(45,gain);							//R45,��ͨ��PGA���� 
	WM8978_Write_Reg(46,gain|1<<8);					//R46,��ͨ��PGA����
}
//WM8978 L2/R2(Ҳ����Line In)��������(L2/R2-->ADC���벿�ֵ�����)
//gain:0~7,0��ʾͨ����ֹ,1~7,��Ӧ-12dB~6dB,3dB/Step
void WM8978_LINEIN_Gain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);							//��ȡR47
	regval&=~(7<<4);												//���ԭ�������� 
 	WM8978_Write_Reg(47,regval|gain<<4);		//����R47
	regval=WM8978_Read_Reg(48);							//��ȡR48
	regval&=~(7<<4);												//���ԭ�������� 
 	WM8978_Write_Reg(48,regval|gain<<4);		//����R48
} 
//WM8978 AUXR,AUXL(PWM��Ƶ����)��������(AUXR/L-->ADC���벿�ֵ�����)
//gain:0~7,0��ʾͨ����ֹ,1~7,��Ӧ-12dB~6dB,3dB/Step
void WM8978_AUX_Gain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);							//��ȡR47
	regval&=~(7<<0);												//���ԭ�������� 
 	WM8978_Write_Reg(47,regval|gain<<0);		//����R47
	regval=WM8978_Read_Reg(48);							//��ȡR48
	regval&=~(7<<0);												//���ԭ�������� 
 	WM8978_Write_Reg(48,regval|gain<<0);		//����R48
}  

//WM8978 ������� 
//dacen:DAC���(����)����(1)/�ر�(0)
//bpsen:Bypass���(¼��,����MIC,LINE IN,AUX��)����(1)/�ر�(0)
void WM8978_Output_Cfg(uint8_t dacen,uint8_t bpsen)
{
	uint16_t regval=0;
	if(dacen)
		regval|=1<<0;													//DAC���ʹ��
	if(bpsen)
	{
		regval|=1<<1;													//BYPASSʹ��
		regval|=5<<2;													//0dB����
	} 
	WM8978_Write_Reg(50,regval);						//R50����
	WM8978_Write_Reg(51,regval);						//R51���� 
}
//���ö���������������
//voll:����������(0~63)
//volr:����������(0~63)
void WM8978_HPvol_Set(uint8_t voll,uint8_t volr)
{
	voll&=0X3F;
	volr&=0X3F;															//�޶���Χ
	if(voll==0)voll|=1<<6;									//����Ϊ0ʱ,ֱ��mute
	if(volr==0)volr|=1<<6;									//����Ϊ0ʱ,ֱ��mute 
	WM8978_Write_Reg(52,voll);							//R52,������������������
	WM8978_Write_Reg(53,volr|(1<<8));				//R53,������������������,ͬ������(HPVU=1)
}
//������������
//voll:����������(0~63)
void WM8978_SPKvol_Set(uint8_t volx)
{
	volx&=0X3F;//�޶���Χ
	if(volx==0)volx|=1<<6;									//����Ϊ0ʱ,ֱ��mute 
 	WM8978_Write_Reg(54,volx);							//R54,������������������
	WM8978_Write_Reg(55,volx|(1<<8));				//R55,������������������,ͬ������(SPKVU=1)
}
//����I2S����ģʽ
//fmt:0,LSB(�Ҷ���);1,MSB(�����);2,�����ֱ�׼I2S;3,PCM/DSP;
//len:0,16λ;1,20λ;2,24λ;3,32λ;
void WM8978_I2S_Cfg(uint8_t fmt,uint8_t len)
{
	fmt&=0x03;
	len&=0x03;															//�޶���Χ
	WM8978_Write_Reg(4,(fmt<<3)|(len<<5));	//R4,WM8978����ģʽ����	
}


