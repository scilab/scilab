/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTSLEEP__
#define __INTSLEEP__

#ifdef _MSC_VER
	#include <windows.h>
#else
	#include <sys/utsname.h>
#endif

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"


int C2F(intsleep) _PARAMS((char *fname,unsigned long fname_len));

#endif /*INTSLEEP*/
/*-----------------------------------------------------------------------------------*/ 
