#include "os.h"

void Task_Sched(void)
{
	EA=0;//关中断 临界段保护
	
	OS_Search();
	if(OSPrioCur == First_Start)
	{
		//第一次启动，切换最高优先级任务
		 SP=OSRdyList[TASK_NUM-1].Task_Sp;
		 __asm POP 7        
		 __asm POP 6
		 __asm POP 5
		 __asm POP 4
		 __asm POP 3
		 __asm POP 2
		 __asm POP 1
		 __asm POP 0
		 __asm POP DPH
		 __asm POP DPL
		 __asm POP PSW
		 __asm POP B
		 __asm POP ACC 
		 OSPrioCur=TASK_NUM-1;
	}
	if( OSRdyList[OSPrioCur].DelayCnt != 0)
	{
		 __asm PUSH ACC       //保护当前任务寄存器
		 __asm PUSH B
		 __asm PUSH PSW
		 __asm PUSH DPL
		 __asm PUSH DPH
		 __asm PUSH 0         //0-7为工作寄存器
		 __asm PUSH 1
		 __asm PUSH 2
		 __asm PUSH 3
		 __asm PUSH 4
		 __asm PUSH 5
		 __asm PUSH 6
		 __asm PUSH 7
		 OSRdyList[OSPrioCur].Task_Sp=SP;   //保存当前堆栈指针

		 SP=OSRdyList[OSPrioHighRdy].Task_Sp;     //任务堆栈指针切换
		 __asm POP 7         //恢复任务寄存器
		 __asm POP 6
		 __asm POP 5
		 __asm POP 4
		 __asm POP 3
		 __asm POP 2
		 __asm POP 1
		 __asm POP 0
		 __asm POP DPH
		 __asm POP DPL
		 __asm POP PSW
		 __asm POP B
		 __asm POP ACC
		 OSPrioCur=OSPrioHighRdy;
	}
		
	
	if(OSPrioHighRdy<OSPrioCur)
	{
		 __asm PUSH ACC       //保护当前任务寄存器
		 __asm PUSH B
		 __asm PUSH PSW
		 __asm PUSH DPL
		 __asm PUSH DPH
		 __asm PUSH 0         //0-7为工作寄存器
		 __asm PUSH 1
		 __asm PUSH 2
		 __asm PUSH 3
		 __asm PUSH 4
		 __asm PUSH 5
		 __asm PUSH 6
		 __asm PUSH 7
		 OSRdyList[OSPrioCur].Task_Sp=SP;   //保存当前堆栈指针

		 SP=OSRdyList[OSPrioHighRdy].Task_Sp;     //任务堆栈指针切换
		 __asm POP 7         //恢复任务寄存器
		 __asm POP 6
		 __asm POP 5
		 __asm POP 4
		 __asm POP 3
		 __asm POP 2
		 __asm POP 1
		 __asm POP 0
		 __asm POP DPH
		 __asm POP DPL
		 __asm POP PSW
		 __asm POP B
		 __asm POP ACC
		 OSPrioCur=OSPrioHighRdy;
	 }
	

	EA=1;
	__asm RETI
}
