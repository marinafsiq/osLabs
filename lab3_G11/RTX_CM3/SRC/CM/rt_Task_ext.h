/*----------------------------------------------------------------------------
 *      ECE254 Lab3
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK_EXT.H
 *      Purpose: Header file for extended task management  
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
/* Definitions */

/* Assigning values for the state 
#define INACTIVE        0
#define READY           1
#define RUNNING         2
#define WAIT_DLY        3
#define WAIT_ITV        4
#define WAIT_OR         5
#define WAIT_AND        6
#define WAIT_SEM        7
#define WAIT_MBX        8
#define WAIT_MUT        9
#define WAIT_MEM				10
*/

/* Structures */
/* added in ECE254 lab3 keil_proc, better to put in a separate file. */
typedef struct rl_task_info {
  U8     state;                   /* Task state                              */
  U8     prio;                    /* Execution priority                      */
  U8     task_id;                 /* Task ID value for optimized TCB access  */
  U8     stack_usage;             /* Stack usage percent value. eg.=58 if 58%*/
  void   (*ptask)();              /* Task entry address                      */ 
} RL_TASK_INFO;



/* Functions */
/* added in ECE254 lab2 keil_proc */
//#define __SVC_0  __svc_indirect(0)

extern int rt_tsk_count_get(void);
//#define os_tsk_count_get()  _os_tsk_count_get((U32)rt_tsk_count_get)
//extern void _os_tsk_count_get (U32 p)  __SVC_0

/* added in ECE254 lab3 keil_proc */
extern OS_RESULT rt_tsk_get(OS_TID task_id, RL_TASK_INFO *p_task_info);
//#define os_tsk_get()  _os_tsk_get((U32)rt_tsk_get)
//extern OS_RESULT _os_tsk_get (U32 p) __SVC_0

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
