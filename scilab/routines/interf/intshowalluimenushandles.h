/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __INTSHOWALLUIMENUSHANDLES__
#define __INTSHOWALLUIMENUSHANDLES__

#include <stdio.h> 
#include <string.h> 
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#include "intcommongraphics.h"
#include "../graphics/ObjectStructure.h"
/*-----------------------------------------------------------------------------------*/
int intshowalluimenushandles(char *fname,unsigned long fname_len);
#endif /* __INTSHOWALLUIMENUSHANDLES__ */
/*-----------------------------------------------------------------------------------*/


