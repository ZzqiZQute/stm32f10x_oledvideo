#include "audio_task.h"
#include "delay.h" 
#include "wave.h"
#include "timer.h"
//������ƿ�
OS_TCB	AudioTaskTCB;
//�����ջ
__align(8) CPU_STK	AUDIO_TASK_STK[AUDIO_STK_SIZE];

//�����������
void audio_task(void *p_arg)
{	//uint32_t temp = 0;
	//uint8_t i = 0;
	//CPU_SR_ALLOC();
	//mp3_play_song("0:audio.mp3");

	wav_play_song("0:audio.wav");
	while(1)
	{
//		OS_CRITICAL_ENTER();	//�����ٽ���
//		printf("float run %d times\r\n",temp++);
//		OS_CRITICAL_EXIT();	//�����ٽ���
//		for( i = 0;i < 5; i++ )
//			printf("float : 67890 \r\n");
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
		
	
		
	}
}
