#ifndef _FLOAT_TASK_H_
#define _FLOAT_TASK_H_


#include "includes.h"

//�������ȼ�
#define FLOAT_TASK_PRIO		4u
//�����ջ��С
#define FLOAT_STK_SIZE		128u
//������ƿ�
extern OS_TCB	FloatTaskTCB;
//�����ջ
extern __align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];


void float_task(void *p_arg);
#endif
