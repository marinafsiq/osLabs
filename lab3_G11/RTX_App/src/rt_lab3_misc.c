#include <LPC17xx.h>
#include "uart_polling.h" 
#include <RTL.h>
#include "../../RTX_CM3/INC/RTL_ext.h" /* extended interface header file */
#include <stdio.h>
#include <string.h>


#define NUM_FNAMES 4
 
char *state2str(unsigned char state, char *str);
char *fp2name(void (*p)(), char *str);

OS_MUT g_mut_uart;
OS_TID g_tid = 255;

int  g_counter = 0;  // a global counter
char g_str[16];
char g_tsk_name[16];

_declare_box (mpool,12,20);

/*--------------------------- task1 -----------------------------------*/
/* checking states of all tasks in the system                          */
/*---------------------------------------------------------------------*/
__task void task1(void)
{
	U8 i=1;
	RL_TASK_INFO task_info;
	
  
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("TID\tNAME\t\tPRIO\tSTATE   \t%%STACK\n");
	os_mut_release(g_mut_uart);
    
	for(i = 0; i < 3 ; i++) { 
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
    
	for(;;);
}

/*--------------------------- task2 -----------------------------------*/
/*  A task can allocate a fixed size of memory                                                                   */
/*---------------------------------------------------------------------*/
__task void task2(void)
{
	int *mptr;
	mptr = (void *)_alloc_box (mpool);
}	

__task void init(void)
{
	os_tsk_create(task1, 1); // task1 at priority 1
	os_tsk_create(task2, 1); // task2 at priority 2
	//os_tsk_create(task3, 1); // task3 at priority 3
 	//os_tsk_create(task4, 1); // task4 at priority 4
	//os_tsk_create(task5, 1); // task5 at priority 5
 	os_tsk_delete_self(); // must delete itself before exiting
}

int main ()
{
	SystemInit();
	uart0_init();
	os_sys_init(init);
}
