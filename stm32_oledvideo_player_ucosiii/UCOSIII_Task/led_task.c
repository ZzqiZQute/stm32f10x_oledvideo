#include "led_task.h"
#include "LED.h"
#include "sys.h"

//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];

//led0������
void led0_task(void *p_arg)
{
//	OS_ERR err;
//	uint32_t temp = 0;
//	uint8_t i = 0;
//	CPU_SR_ALLOC();
//	p_arg = p_arg;
//	while(1)
//	{	
////		OS_CRITICAL_ENTER();	//�����ٽ���
////	//	printf("LED run %d times\r\n",temp++);
////		OS_CRITICAL_EXIT();	//�����ٽ���
//////		
////		for( i = 0;i < 5; i++ )
////			printf("LED : 12345 \r\n");
//		LED0=1-LED0;
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
//	}
//}

////		printf("LED ���� %d ��\r\n",temp++);
//		LED0 = 0;
//		OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
//		LED0 = 1;
//		OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms

}
