#ifndef AUDIO_TASK_H_
#define AUDIO_TASK_H_


#include "includes.h"

//�������ȼ�
#define AUDIO_TASK_PRIO		4u
//�����ջ��С	
#define AUDIO_STK_SIZE 		128u
//������ƿ�
extern OS_TCB AudioTaskTCB;
//�����ջ	
extern CPU_STK AUDIO_TASK_STK[AUDIO_STK_SIZE];

void audio_task(void *p_arg);

#endif
