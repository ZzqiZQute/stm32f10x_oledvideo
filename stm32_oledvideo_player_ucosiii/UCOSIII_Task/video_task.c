#include "video_task.h"
#include "delay.h"
#include "stdio.h"
#include "oled_iic.h"
#include "timer.h"
#include "ff.h"
#include "malloc.h"
#include "read.h"
OS_TCB	VideoTaskTCB;

__align(8) CPU_STK	VIDEO_TASK_STK[VIDEO_STK_SIZE];

unsigned char buff[1024];
unsigned char lastbuff[1024];
u32 iii=0;
void video_task(void *p_arg)
{

	OS_ERR err;
	FIL* fvideo;
	int currentFrameGuess;
  int lastFrame = -1;
	double tpf;
	u32 startTime;
	UINT readcnt;
	//CPU_SR_ALLOC();
	//uint32_t temp = 0;
	//uint8_t i = 0;
	OSTimeDlyHMSM(0,0,0,250,OS_OPT_TIME_HMSM_STRICT,&err);
	fvideo=(FIL*)mymalloc(sizeof(FIL));//ÉêÇëÄÚ´æ 				 		   		 						  
	my_f_open(fvideo,"0:video.bin",FA_READ);//´ò¿ªÎÄ¼þ
	my_f_read(fvideo,&tpf,8,&readcnt);

	Timer_Start();	
	startTime=Get_Tick();
	while(1)
	{
		currentFrameGuess = (int)((Get_Tick()-startTime)/tpf);
		my_f_lseek(fvideo,8+(currentFrameGuess)*1024);
		my_f_read(fvideo,buff,1024,&readcnt);
		if(readcnt!=1024){
			break;
		}	
	  if(lastFrame == -1)
		{	
			OLED_DrawFullImage((u8*)buff);
		}
		else
		{
			OLED_DrawDifference(buff,lastbuff);
			
		}
		lastFrame = currentFrameGuess;
		memcpy(lastbuff,buff,1024);

		
	///iii++;
	//	while(1);
	///	memset(buff,iii%2?255:0,1024);
	///	
	
		
		
	}
}
