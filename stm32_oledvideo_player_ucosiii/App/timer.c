#include "timer.h"
//#include "ctrl.h"
u32 tick=0;
void Timer_Init(void){
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //??TIM3??
    
    TIM_TimeBaseInitStructure.TIM_Period = 9;     //??????
    TIM_TimeBaseInitStructure.TIM_Prescaler=7199;  //?????
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //????
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//???TIM3
    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //??TIM3????
    TIM_Cmd(TIM4,ENABLE); //??TIM3
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //???3??
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //?????1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //?????3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
void TIM4_IRQHandler(void) 
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update );
				tick++;
			//  handleCtrl();
		}
}
void Timer_Start(void){
		TIM_Cmd(TIM4,ENABLE);
}
void Timer_Stop(void){
		TIM_Cmd(TIM4,DISABLE);
}
u32 Get_Tick(void)
{
	return tick;
	
}
