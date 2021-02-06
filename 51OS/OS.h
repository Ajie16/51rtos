/*----------------------------------------------------------
 *	OS.h
 *	系统头文件
 *  Author：徐
 *  Date：2020.9.29
 *  Describe:Include the head file of 51RTOS
-----------------------------------------------------------*/
 
#ifndef __OS_H
#define __OS_H
#include <reg52.H>

//优先级0 - TASK_NUM-1 不能 超过 或 重复

//-----------------The max task-----------------------------
#define OS_MAX_TASKS 8
#define TASK_NUM 3 //建立的任务数量
#define NONE_Rdy 10//无任务
#define First_Start 11//空闲任务

#define       OS_IdleTaskStkSize       15
//--------------------CPU重定义区域-------------------------
typedef unsigned char   CPU_INT08U;
typedef unsigned short  CPU_INT16U;

typedef unsigned char   u8;
typedef unsigned short  u16;

//--------------------堆栈数据类型重定义--------------------
typedef CPU_INT08U      CPU_STK;
typedef CPU_INT08U      CPU_STK_SIZE;
typedef CPU_INT08U      CPU_PRIO;
typedef short     		  CPU_DELAY_CNT;
//-------------------用户预留操作区------------------

//-------------------用户预留操作区------------------

//-------------------任务创建关联有关定义------------------
typedef struct  os_tcb          OS_TCB;
typedef void    (*OS_TASK_PTR)  (void *p_arg);//函数指针

struct os_tcb
{
	CPU_STK Task_Sp;
	CPU_STK *StkPtr;
	CPU_STK_SIZE StkSize;
	CPU_PRIO Prio;//优先级
	CPU_DELAY_CNT DelayCnt;
};

//---------------------就绪列表有关定义------------------
//就绪列表
extern OS_TCB    OSRdyList[TASK_NUM];

//--------------------数据变量定义区--------------------


//--------------------优先级及就绪列表------------------

extern CPU_PRIO OSPrioTbl;//任务优先级表（8位控制）
extern CPU_PRIO OSPrioCur;//当前优先级
extern CPU_PRIO OSPrioHighRdy;//就绪最高优先级

//---------------空闲任务-------------------
extern OS_TCB        OS_IdleTaskTcb;
extern CPU_STK       OS_IdleTaskStk[OS_IdleTaskStkSize];


void PrioInsert(CPU_PRIO prio);
void PrioRemove(CPU_PRIO prio);

//---------------------任务切换定义----------------------
void Task_Sched(void);

//--------------------错误枚举------------------------------

//----------------------时间戳-------------------------------
void OS_SysTickInit(void);
void OS_DelayCheck(void);

void delay_ms(CPU_DELAY_CNT cnt);
//--------------------任务创建函数--------------------------

void OSTaskCreat(
								OS_TCB         *p_tcb,
								unsigned int   p_task,
								CPU_PRIO       prio,
								CPU_STK        *p_stk_base,
								CPU_STK_SIZE   stk_size
								);

								
//--------------------堆栈初始化函数------------------------



//---------------------空闲函数有关--------------------------



void          OS_IdleTask(void);

//---------------------就绪列表有关函数------------------------------	

void          OS_RdyListInit            (void);
void          OS_RdyListInsert          (OS_TCB       *p_tcb);
//void          OS_RdyListRemove          (OS_TCB       *p_tcb);

void          OS_Search                 (void);  //索引最高优先级，赋值给HighRdy

#endif

