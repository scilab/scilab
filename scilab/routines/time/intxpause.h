/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTXPAUSE__
#define __INTXPAUSE__

#ifdef _MSC_VER
	#include <windows.h>
#else
	#include <sys/utsname.h>
#endif

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"


int C2F(intxpausep) _PARAMS((char *fname,unsigned long fname_len));

#endif /*INTXPAUSE*/
/*-----------------------------------------------------------------------------------*/ 
