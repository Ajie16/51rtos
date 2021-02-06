/*----------------------------------------------------------
 *	OS.C
 *	系统内核函数
 *  Author：徐
 *  Date：2020.9.29
 *  Describe:Include the main part of 51RTOS
-----------------------------------------------------------*/
#include <os.h>

//变量定义区域
OS_TCB        OSRdyList[TASK_NUM];
CPU_PRIO      OSPrioTbl;//任务优先级表（8位控制）
CPU_PRIO      OSPrioCur;//当前优先级
CPU_PRIO      OSPrioHighRdy;//就绪最高优先级
OS_TCB        OS_IdleTaskTcb;
CPU_STK       OS_IdleTaskStk[OS_IdleTaskStkSize];


void PrioInsert(CPU_PRIO prio)//置位优先级标志
{
	u8 x;
	x=1<<prio;
	OSPrioTbl |= x;
}

void PrioRemove(CPU_PRIO prio)//清除优先级标志
{
	u8 x;
	x=1<<prio;
	OSPrioTbl &= ~x;
}

void OS_RdyListInit(void)//就绪表清空
{
	u8 i;
	for(i=0;i<TASK_NUM;i++)
	{
		OSRdyList[i].DelayCnt=0;
		OSRdyList[i].Task_Sp=0;
		OSRdyList[i].StkPtr=0;
		OSRdyList[i].StkSize=0;
		OSRdyList[i].Prio=0;//优先级
	}
	OSPrioCur=First_Start;
}
void OS_RdyListInsert(OS_TCB *p_tcb)//TCB插入就绪表
{
	PrioInsert(p_tcb->Prio);

	OSRdyList[p_tcb->Prio].DelayCnt=p_tcb -> DelayCnt;
	OSRdyList[p_tcb->Prio].Task_Sp=p_tcb -> Task_Sp;
	OSRdyList[p_tcb->Prio].StkPtr=p_tcb -> StkPtr;
	OSRdyList[p_tcb->Prio].StkSize=p_tcb -> StkSize;
	OSRdyList[p_tcb->Prio].Prio=p_tcb -> Prio;
}


void OS_DelayCheck(void)
{
	u8 i;
	for(i=0;i<TASK_NUM-1;i++)
	{
		OSRdyList[i].DelayCnt--;
		if(OSRdyList[i].DelayCnt<=0)
		{
			PrioInsert(OSRdyList[i].Prio);
			OSRdyList[i].DelayCnt=0;
		}
	}
	PrioInsert(TASK_NUM-1);

}

void delay_ms(CPU_DELAY_CNT cnt)
{
	OSRdyList[OSPrioCur].DelayCnt=cnt;
	PrioRemove(OSRdyList[OSPrioCur].Prio);

}


void OS_Search(void)
{
	u8 i=0;
		
	for(i=0;i<TASK_NUM;i++)
	{
		if(((OSPrioTbl>>i)&0x01)==1)
		{
			OSPrioHighRdy=i;
			break;
		}
	}
	
}
void OSTaskCreat(
								OS_TCB         *p_tcb,
								unsigned int   p_task,
								CPU_PRIO       prio,
								CPU_STK        *p_stk_base,
								CPU_STK_SIZE   stk_size
								)
{
	p_tcb->Task_Sp=p_stk_base;
  p_stk_base[0]=p_task;        //将任务入口地址保存在堆栈，压入低8位
  p_tcb->Task_Sp++;                         //压入一个后，堆栈加1，因为51的堆栈是往上生长的
  p_stk_base[1]=p_task>>8;     //压入高8位
  p_tcb->Task_Sp+=13;   //设置好堆栈指针
												//即任务开始进入时，堆栈要模仿成被切换返回时的样子
												//这时寄存器是被压栈的，且是压入了Num_PUSH_bytes个
	p_tcb->StkPtr=p_stk_base;
	p_tcb->StkSize=stk_size;
	p_tcb->Prio=prio;
	
	OS_RdyListInsert(p_tcb);
	
	
}

void OS_SysTickInit(void)
{
	EA=1;
	TMOD |= 0x10;
	TH1=0xFC;
	TL1=0x18;
	TR1=1;
	ET1=1;
}

void SysTick() interrupt 3
{
	TH1=0xFC;
	TL1=0x18;
	EA=0;
	OS_DelayCheck();
	Task_Sched();
}