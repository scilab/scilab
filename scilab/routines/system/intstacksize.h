/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTSTACKSIZE__
#define __INTSTACKSIZE__

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"


#ifndef NULL
#define NULL 0
#endif

#define TRUE  1
#define FALSE 0

int C2F(intstacksize) __PARAMS((char *fname,unsigned long fname_len));
/**
/* stacksize - set scilab stack size 

* Calling Sequence 

* stacksize(n)   
* stacksize('max')   
* sz=stacksize()   

* Parameters 

* n : integer, the required stack size given in number of double precision words 
* 'max' : try to allocate the maximum of memory
* sz : 2-vector [total used] 

*/
#endif /* __INTSTACKSIZE__ */
/*-----------------------------------------------------------------------------------*/ 
