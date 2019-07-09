#ifndef __TIMER_H
#define __TIMER_H

#include <stm32f10x.h>
void Timer_Init(void);
void Timer_Start(void);
void Timer_Stop(void);
u32 Get_Tick(void);
#endif
