#ifndef VIDEO_TASK_H_
#define VIDEO_TASK_H_


#include "includes.h"


#define VIDEO_TASK_PRIO		4u

#define VIDEO_STK_SIZE 		128u

extern OS_TCB VideoTaskTCB;

extern CPU_STK VIDEO_TASK_STK[VIDEO_STK_SIZE];

void video_task(void *p_arg);

#endif
