#include "start_task.h"
#include "timer.h"
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];


//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()


//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��� ��1ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
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

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			
								 
	OS_CRITICAL_EXIT();	//�����ٽ���
				 
	OSTaskDel((OS_TCB*)&StartTaskTCB,&err);
}
