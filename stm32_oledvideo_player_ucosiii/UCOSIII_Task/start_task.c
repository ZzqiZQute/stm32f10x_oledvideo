#include "start_task.h"
#include "timer.h"
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];


//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()


//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍 即1ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
	OSTaskCreate((OS_TCB 	* )&VideoTaskTCB,		
				 (CPU_CHAR	* )"video task", 		
                 (OS_TASK_PTR )video_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )VIDEO_TASK_PRIO,     	
                 (CPU_STK   * )&VIDEO_TASK_STK[0],	
                 (CPU_STK_SIZE)VIDEO_STK_SIZE/10,	
                 (CPU_STK_SIZE)VIDEO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )1,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);			
								 
	OSTaskCreate((OS_TCB 	* )&AudioTaskTCB,		
				 (CPU_CHAR	* )"audio task", 		
                 (OS_TASK_PTR )audio_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )AUDIO_TASK_PRIO,     	
                 (CPU_STK   * )&AUDIO_TASK_STK[0],	
                 (CPU_STK_SIZE)AUDIO_STK_SIZE/10,	
                 (CPU_STK_SIZE)AUDIO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )1,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);			

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			
								 
	OS_CRITICAL_EXIT();	//进入临界区
				 
	OSTaskDel((OS_TCB*)&StartTaskTCB,&err);
}
