/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTGETDATE__
#define __INTGETDATE__

#ifdef WIN32
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

int C2F(intgetdate) _PARAMS((char *fname,unsigned long fname_len));

#endif /* __INTGETDATE__ */
/*-----------------------------------------------------------------------------------*/ 
