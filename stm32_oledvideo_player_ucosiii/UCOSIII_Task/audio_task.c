#include "audio_task.h"
#include "delay.h" 
#include "wave.h"
#include "timer.h"
//任务控制块
OS_TCB	AudioTaskTCB;
//任务堆栈
__align(8) CPU_STK	AUDIO_TASK_STK[AUDIO_STK_SIZE];

//浮点测试任务
void audio_task(void *p_arg)
{	//uint32_t temp = 0;
	//uint8_t i = 0;
	//CPU_SR_ALLOC();
	//mp3_play_song("0:audio.mp3");

	wav_play_song("0:audio.wav");
	while(1)
	{
//		OS_CRITICAL_ENTER();	//进入临界区
//		printf("float run %d times\r\n",temp++);
//		OS_CRITICAL_EXIT();	//进入临界区
//		for( i = 0;i < 5; i++ )
//			printf("float : 67890 \r\n");
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
		
	
		
	}
}
