#include "ctrl.h"
#include "stm32f10x_pwr.h"
_ctrl ctrl;
u8 debounceRecord = 0;
u8 debouncePlay = 0;
void Ctrl_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//PA8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA8
	
	ctrl.lastRecordValue = 1;
	ctrl.lastPlayValue = 1;
	ctrl.recordStateChanged=0;
	ctrl.playStateChanged = 0;
}
void RecordOnHint()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
void RecordOffHint()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}
void handleCtrl(){
	handleRecordState();
	handlePlayState();
	
}
void handleRecordState()
{
	u8 data = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
	if(data != ctrl.lastRecordValue)
	{	//����
		debounceRecord ++;
		if(debounceRecord >100){
			debounceRecord =0;
			if(ctrl.recordStateChanged!=0)
			{
					ctrl.recordStateChanged(data);
			}
			ctrl.lastRecordValue=data;
		}
	}else{
		debounceRecord = 0;
	}
}
void handlePlayState()
{
	u8 data = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);
	if(data != ctrl.lastPlayValue)
	{	//����
		debouncePlay ++;
		if(debouncePlay >100){
			debouncePlay =0;
			if(ctrl.playStateChanged!=0)
			{
					ctrl.playStateChanged(data);
			}
			ctrl.lastPlayValue=data;
		}
	}else{
		debouncePlay = 0;
	}
}
void Pwr_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA15
}
void SleepMode()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_WakeUpPinCmd(ENABLE);
	PWR_EnterSTANDBYMode();
}
