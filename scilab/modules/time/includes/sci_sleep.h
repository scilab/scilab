/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __SCI_SLEEP__
#define __SCI_SLEEP__

#ifdef _MSC_VER
	#include <windows.h>
#else
	#include <sys/utsname.h>
	#include <unistd.h>
#endif


#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"


int C2F(sci_sleep) _PARAMS((char *fname,unsigned long fname_len));

#endif /*SCI_SLEEP*/
/*--------------------------------------------------------------------------*/ 
