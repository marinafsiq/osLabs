/*----------------------------------------------------------------------------
 *      ECE254 Lab3
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_EXT.H
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
/* Variables */

/* Functions */
//#define __SVC_0         __svc_indirect(0)

extern OS_RESULT rt_free_box_s  (void *p_mpool, void *ptr);
//#define os_free_box_s(p_mpool,box)  _os_free_box_s((U32)rt_free_box_s, p_mpool,box);
//extern OS_RESULT _os_free_box_s(U32 p, void *p_mpool, void *box)         __SVC_0;

extern void *rt_alloc_box_s (void *p_mpool);
//#define os_alloc_box_s(p_mpool)  _os_alloc_box_s((U32)rt_alloc_box_s, p_mpool);
//extern void _os_alloc_box_s(U32 p, void *p_mpool)         __SVC_0;


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

