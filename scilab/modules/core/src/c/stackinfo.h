/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __STACKINFO_H__
#define __STACKINFO_H__
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
/*-----------------------------------------------------------------------------------*/ 
integer C2F(getstackinfo)(integer *total,integer *used);
integer C2F(getgstackinfo)(integer *total,integer *used);

BOOL is_a_valid_size_for_scilab_stack(int sizestack);
unsigned long get_max_memory_for_scilab_stack(void);
#endif /*__STACKINFO_H__*/
/*-----------------------------------------------------------------------------------*/ 

