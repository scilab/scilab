/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GETMEMORYSIZE__
#define __GETMEMORYSIZE__

#ifdef WIN32
	#include <windows.h>
#else
	#if defined(hpux)
		#include <sys/param.h>
		#include <sys/pstat.h>
	#else
		#if defined(__APPLE__) 
			#include <sys/types.h>
			#include <sys/sysctl.h>
		#else	 /* Linux ,Solaris and others */
			#include <unistd.h>
		#endif
	#endif
#endif

#include <stdio.h>
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"

int C2F(intgetmemorysize) _PARAMS((char *fname));

#endif /*GETMEMORYSIZE*/
/*-----------------------------------------------------------------------------------*/ 
