/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTPUSHFFTW_PLAN__
#define __INTPUSHFFTW_PLAN__

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int sci_pushfftw_plan __PARAMS((char *fname,unsigned long fname_len));

#endif /* __INTPUSHFFTW_PLAN__ */
/*-----------------------------------------------------------------------------------*/
