/**
 * @brief: ECE254 lab3 starter file that calls os_tsk_get()
 * @file: lab3_main.c
 * @date: 2014/05/28
 */
/* NOTE: This release is for students to use */

#include <LPC17xx.h>
#include "uart_polling.h" 
#include <RTL.h>
#include "../../RTX_CM3/INC/RTL_ext.h" /* extended interface header file */
#include <stdio.h>
#include <string.h>
#include "../../RTX_CM3/SRC/CM/RTX_Config.h"

#define NUM_FNAMES 8

struct func_info {
  void (*p)();      /* function pointer */
  char name[16];    /* name of the function */
};

extern void os_idle_demon(void);
//__task void task1(void);
__task void task2(void);
__task void task3(void);
__task void task4(void);
__task void task5(void);
__task void task6(void);
__task void init (void);
 
const U32 msize = sizeof(U64);
const U32 cnt = 10;

_declare_box8(mympool, msize, cnt);

char *state2str(unsigned char state, char *str);
char *fp2name(void (*p)(), char *str);

OS_MUT g_mut_uart;
OS_TID g_tid = 255;

int  g_counter = 0;  // a global counter
char g_str[16];
char g_tsk_name[16];

struct func_info g_task_map[NUM_FNAMES] = \
{
  /* os_idle_demon function ptr to be initialized in main */
  {NULL,  "os_idle_demon"}, \
/*  {task1, "task1"},   \*/
  {task2, "task2"},   \
  {task3, "task3"},   \
  {task4, "task4"},   \
  {task5, "task5"},   \
  {task6, "task6"},   \
  {init,  "init" }
};

/* no local variables defined, use one global var */
/*__task void task1(void)
{
	for (;;) {
		g_counter++;
	}
}
*/

/*--------------------------- task2 -----------------------------------*/
/* checking states of all tasks in the system                          */
/*---------------------------------------------------------------------*/

__task void task2(void)
{
	U8 i=1;
	RL_TASK_INFO task_info;
	
 for(;;)
 {
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("TID\tNAME\t\tPRIO\tSTATE   \t%%STACK\n");
	os_mut_release(g_mut_uart);
    
	for(i = 0; i < os_maxtaskrun; i++) { // this is a lazy way of doing loop.
		if (os_tsk_get(i+1, &task_info) == OS_R_OK) {
			os_mut_wait(g_mut_uart, 0xFFFF);  
			printf("%d\t%s\t\t%d\t%s\t%d%%\n", \
			       task_info.task_id, \
			       fp2name(task_info.ptask, g_tsk_name), \
			       task_info.prio, \
			       state2str(task_info.state, g_str),  \
			       task_info.stack_usage);
			os_mut_release(g_mut_uart);
		} 
	}
    
	if (os_tsk_get(0xFF, &task_info) == OS_R_OK) {
		os_mut_wait(g_mut_uart, 0xFFFF);  
		printf("%d\t%s\t\t%d\t%s\t%d%%\n", \
		       task_info.task_id, \
		       fp2name(task_info.ptask, g_tsk_name), \
		       task_info.prio, \
		       state2str(task_info.state, g_str),  \
		       task_info.stack_usage);
		os_mut_release(g_mut_uart);
	}			
 }	
}


/*--------------------------- task3 -----------------------------------*/
/*  A task can allocate a fixed size of memory                                                                   */
/*---------------------------------------------------------------------*/
__task void task3(void)
{

	const int mpool_size = 8;
	void* mpool[mpool_size];
	int i;
	
	for(i = 0; i<mpool_size;i++) {
		printf("Allocating memory block %d\n",i);
		mpool[i] = os_mem_alloc(&mympool);
		printf("Finished Allocating block %d",i);
	}
	
	
	for(i = 0;i<mpool_size;i++) {
		printf("Freeing memory block %d",i);
		os_mem_free(&mympool,mpool[i]);
	}
	
}	

/*--------------------------- task4 -----------------------------------*/
/* A task will get blocked if there is no memory available when        */
/*   os_mem_alloc() icalled.                                           */
/*---------------------------------------------------------------------*/
__task void task4(void)
{

	const int mpool_size = 8;
	void* mpool[mpool_size];
	int i;
	
	for(i = 0; i<mpool_size;i++) {
		printf("Allocating memory block %d\n",i);
		mpool[i] = os_mem_alloc(&mympool);
		printf("Finished Allocating block %d",i);
	}
	
	
	
	for(i = 0;i<mpool_size;i++) {
		printf("Freeing memory block %d",i);
		os_mem_free(&mympool,mpool[i]);
	}
	
}	

/*--------------------------- task5 -----------------------------------*/
/* A blocked on memory task will be resumed once enough memory is      */
/*  available in thesystem.                                            */
/*---------------------------------------------------------------------*/
__task void task5(void)
{

	const int mpool_size = 8;
	void* mpool[mpool_size];
	int i;
	
	for(i = 0; i<mpool_size;i++) {
		printf("Allocating memory block %d\n",i);
		mpool[i] = os_mem_alloc(&mympool);
		printf("Finished Allocating block %d",i);
	}
	
	
	
	for(i = 0;i<mpool_size;i++) {
		printf("Freeing memory block %d",i);
		os_mem_free(&mympool,mpool[i]);
	}
	
}	

/*--------------------------- task6 -----------------------------------*/
/* Create a testing scenario that multiple tasks with dfferent priorities are all blocked */
/*waiting for memory. When memory becomes available, test whether it is the highest*/
/*priority task that waits the longest that gets the memory first.*/                                
/*---------------------------------------------------------------------*/
__task void task6(void)
{

	const int mpool_size = 8;
	void* mpool[mpool_size];
	int i;
	
	for(i = 0; i<mpool_size;i++) {
		printf("Allocating memory block %d\n",i);
		mpool[i] = os_mem_alloc(&mympool);
		printf("Finished Allocating block %d",i);
	}
	
	
	
	for(i = 0;i<mpool_size;i++) {
		printf("Freeing memory block %d",i);
		os_mem_free(&mympool,mpool[i]);
	}
	
}	

/*--------------------------- init ------------------------------------*/
/* initialize system resources and create other tasks                  */
/*---------------------------------------------------------------------*/
__task void init(void)
{
	const int mpool_size = 8;
	void* mpool[mpool_size];
	int i;

	_init_box8(&mympool, msize*cnt, msize);
	
	os_mut_init(&g_mut_uart);
  
	
	//Using up the entire memory
	for (i = 0; i < mpool_size; i++) {
		mpool[i] = os_mem_alloc(&mympool);
		printf("Allocating Memory Block %d\n", i);
	}	
	
  
	os_mut_wait(g_mut_uart, 0x00FF);
	printf("init: TID = %d\n", os_tsk_self());
	os_mut_release(g_mut_uart);
  
	/*g_tid = os_tsk_create(task1, 7);  // task 1 has a  prio = 7 
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task1 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);*/
  
	g_tid = os_tsk_create(task2, 1);  // task 2 has a  prio = 1 
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("task2 created with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task3, 3);  // task 3 has a  prio = 3  
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("task3 created with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task4, 2);  // task 4 has a  prio = 2 
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("task4 created with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task5, 4);  // task 5 has a  prio = 4 
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("task5 created with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task6, 4);  // task 6 has a  prio = 4 
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("task6 created with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	printf("Entering Delay");
	os_dly_wait(5);
	
	printf("Freeing Allocated Memory Blocks\n");
 	for (i = 0; i < mpool_size; i++) {
    os_mem_free(&mympool, mpool[i]);
	}
  
	os_tsk_delete_self();     /* task MUST delete itself before exiting */
}


/**
 * @brief: convert state numerical value to string represenation      
 * @param: state numerical value (macro) of the task state
 * @param: str   buffer to save the string representation, 
 *               buffer to be allocated by the caller
 * @return:the string starting address
 */
char *state2str(unsigned char state, char *str)
{
	switch (state) {
	case INACTIVE:
		strcpy(str, "INACTIVE");
		break;
	case READY:
		strcpy(str, "READY   ");
		break;
	case RUNNING:
		strcpy(str, "RUNNING ");
		break;
	case WAIT_DLY:
		strcpy(str, "WAIT_DLY");
		break;
	case WAIT_ITV:
		strcpy(str, "WAIT_ITV");
		break;
	case WAIT_OR:
		strcpy(str, "WAIT_OR");
		break;
	case WAIT_AND:
		strcpy(str, "WAIT_AND");
		break;
	case WAIT_SEM:
		strcpy(str, "WAIT_SEM");
		break;
	case WAIT_MBX:
		strcpy(str, "WAIT_MBX");
		break;
	case WAIT_MUT:
		strcpy(str, "WAIT_MUT");
		break;
	default:
		strcpy(str, "UNKNOWN");    
	}
	return str;
}

/** 
 * @brief: get function name by function pointer
 * @param: p the entry point of a function (i.e. function pointer)
 * @param: str the buffer to return the function name
 * @return: the function name string starting address
 */
char *fp2name(void (*p)(), char *str)
{
	int i;
	unsigned char is_found = 0;
  
	for ( i = 0; i < NUM_FNAMES; i++) {
		if (g_task_map[i].p == p) {
			str = strcpy(str, g_task_map[i].name);  
			is_found = 1;
			break;
		}
	}
	if (is_found == 0) {
		strcpy(str, "ghost");
	}
	return str;
}

int main(void)
{
	SystemInit();         /* initialize the LPC17xx MCU */
	uart0_init();         /* initilize the first UART */
  
  
	/* fill the fname map with os_idle_demon entry point */
	g_task_map[0].p = os_idle_demon;
  
	printf("Calling os_sys_init()...\n");
	os_sys_init(init);    /* initilize the OS and start the first task */
}
