#ifndef AUDIO_TASK_H_
#define AUDIO_TASK_H_


#include "includes.h"

//任务优先级
#define AUDIO_TASK_PRIO		4u
//任务堆栈大小	
#define AUDIO_STK_SIZE 		128u
//任务控制块
extern OS_TCB AudioTaskTCB;
//任务堆栈	
extern CPU_STK AUDIO_TASK_STK[AUDIO_STK_SIZE];

void audio_task(void *p_arg);

#endif
