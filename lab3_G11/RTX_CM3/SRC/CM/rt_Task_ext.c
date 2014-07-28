/*----------------------------------------------------------------------------
 *      ECE254 Lab3
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK_ext.C
 *      Purpose: Interface functions for extended task management  
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"
#include "rt_MemBox_ext.h"
#include "rt_Mailbox.h"

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/
/*--------------------------- rt_tsk_count_get ------------------------------*/
/* added in ECE254 lab2 keil_rtx */
int rt_tsk_count_get (void) {
	/* Add your own code here. Change the following line accordingly */
	P_TCB p_task;
  int i = 0;
	/* Initialize the counter variable to 0 */
	int tsk_count = 0;
	
	for(i = 0; i <= os_maxtaskrun ; i++)
	{
		p_task = os_active_TCB[i];
	
		/* If the task is INACTIVE or null, it will not be counted */
		if (p_task->state != INACTIVE && p_task != NULL)
			tsk_count++;
		
	}
  return tsk_count;	
}

/*--------------------------- rt_tsk_get ------------------------------------*/
/* added in ECE254 lab3 keil_proc */
OS_RESULT rt_tsk_get (OS_TID task_id, RL_TASK_INFO *p_task_info) {
	/* Add your own code here. Change the following lines accordingly */	
	P_TCB p_task;
	double stk_start;
	double stk_end;
	double stk_diff;
	double stk_usage;

	/* Check if the task id is not valid or not started */
	if(task_id > os_maxtaskrun || os_active_TCB[task_id-1] == NULL) 
	     return (OS_R_NOK);
	p_task = os_active_TCB[task_id - 1];
	p_task_info->task_id     = task_id;
	p_task_info->state       = p_task->state;
	p_task_info->prio        = p_task->prio;
	/* Calculating the usage of the stack */
	stk_start = (U32)&p_task->stack[(U16)os_stackinfo/4];		
	if(p_task->state == RUNNING)	
	{
	  stk_end = (U32)rt_get_PSP();
	}
	else
	{
	  stk_end =  (U32)p_task->tsk_stack;
        }
	stk_diff = (float) (stk_start - stk_end);	
	stk_usage =  (stk_diff/((U16) os_stackinfo ))*100;
	p_task_info->stack_usage = stk_usage;
	p_task_info->ptask       = p_task->ptask;
	return (OS_R_OK);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
