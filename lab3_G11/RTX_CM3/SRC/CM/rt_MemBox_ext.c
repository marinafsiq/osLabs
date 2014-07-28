/*----------------------------------------------------------------------------
 *      ECE254 Lab3
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_ext.C
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *      Includes
 *---------------------------------------------------------------------------*/
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_List.h"
#include "rt_Robin.h"
#include "rt_MemBox.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"
#include "rt_Task.h"       /* added in ECE254 lab3 keil_proc */ 
#include "rt_MemBox_ext.h" /* added in ECE254 lab3 keil_proc */
   

/* ECE254 Lab3 Comment: You may need to include more header files */

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/
struct OS_XCB p_CB = {TCB,NULL,NULL,NULL,NULL,0};

/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*==========================================================================*/
/*  The following are added for ECE254 Lab3 Proc Management Assignmet       */
/*==========================================================================*/

/*---------------- rt_alloc_box_s, task blocks when out of memory-----------*/

/**  
   @brief: Blocking memory allocation routine.
 */
void *rt_alloc_box_s (void *p_mpool) {
	
	/* Add your own code here. Change the following line accordingly */
   void *ptr;
   ptr	= rt_alloc_box (p_mpool);
	
   if(ptr == NULL)
   {
	   rt_put_prio(&p_CB, os_tsk.run);
	   rt_block(0xFFFF, 10);
	   return ptr;
   }
   else
	   return ptr;
}


/*----------- rt_free_box_s, pair with _s memory allocators ----------------*/
/**
 * @brief: free memory pointed by ptr, it will unblock a task that is waiting
 *         for memory.
 * @return: OS_R_OK on success and OS_R_NOK if ptr does not belong to gp_mpool 
 */
OS_RESULT rt_free_box_s (void *p_mpool, void *box) {
	/* Add your own code here. Change the following line accordingly */
	P_TCB p_task;
	
  if(p_CB.p_lnk != NULL){
	  p_task = rt_get_first(&p_CB);
	  p_task->ret_val = (U32)box;
		p_task->state = READY;
	  rt_dispatch(p_task);
	  return OS_R_OK;
  }
	
  else{
	  if(rt_free_box(p_mpool,box))
		  return OS_R_NOK;
	  else
		  return OS_R_OK;
   }	 
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
