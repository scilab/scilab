/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCI_XPAUSE__
#define __SCI_XPAUSE__

#ifdef _MSC_VER
	#include <windows.h>
#else
	#include <sys/utsname.h>
#endif

#include <stdio.h>
#include "../../core/includes/machine.h"
#include "../../core/includes/stack-c.h"
#include "../../core/includes/version.h"



int C2F(sci_xpause) _PARAMS((char *fname,unsigned long fname_len));

#endif /*SCI_XPAUSE*/
/*-----------------------------------------------------------------------------------*/ 
