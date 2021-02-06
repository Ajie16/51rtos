#include "app.h"
#include "OS.h"
#include "led.h"
#include "bsp.h"

#define TASK1_STK_SIZE 15
#define TASK2_STK_SIZE 15

static CPU_STK Task1_Stk[TASK1_STK_SIZE];
static CPU_STK Task2_Stk[TASK2_STK_SIZE];

static OS_TCB  Task1Tcb;
static OS_TCB  Task2Tcb;

void task1(void);
void task2(void);

sbit LED0=P2^0;
sbit LED1=P2^1;
void main(void)
{
	OS_SysTickInit();
	OS_RdyListInit();
								
	OSTaskCreat(
							&Task1Tcb,
							(unsigned int)   &task1,
							1,
							&Task1_Stk,
							TASK1_STK_SIZE
						 );	

								
	OSTaskCreat(
							&Task2Tcb,
							(unsigned int)   &task2,
							0,
							&Task2_Stk,
							TASK2_STK_SIZE
						 );
							
		OSTaskCreat(
							&OS_IdleTaskTcb,
							(unsigned int)   &OS_IdleTask,
							2,
							&OS_IdleTaskStk,
							OS_IdleTaskStkSize
						 );	
	while(1)
	{
		LED1=~LED1;
	}
}

void task1(void) 
{
		 static unsigned char OK_flag1;
	while(1)
	{
		LED1=~LED1;
		delay_ms(500);
	}
}


void task2(void)
{
		 static unsigned char OK_flag2;
	while(1)
	{
		LED0=~LED0;
		delay_ms(1000);
	}
}


void  OS_IdleTask(void)
{
	 static unsigned char OK_flag;
	while(1)
	{
		for(OK_flag=0;OK_flag<100;OK_flag++);
	}
}

