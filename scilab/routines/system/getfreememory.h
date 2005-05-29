/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GETFREEMEMORY__
#define __GETFREEMEMORY__

#ifdef WIN32
	#include <windows.h>
#else
	#if defined(hpux)
		#include <sys/param.h>
		#include <sys/pstat.h>
	#else
		#include <unistd.h>
	#endif
#endif

#include <stdio.h>
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"

int C2F(intgetfreememory) _PARAMS((char *fname));

#endif /*GETFREEMEMORY*/
/*-----------------------------------------------------------------------------------*/ 
