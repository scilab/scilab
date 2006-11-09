/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTFFTW_INPLACE__
#define __INTFFTW_INPLACE__

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int sci_fftw_inplace __PARAMS((char *fname,unsigned long fname_len));

#endif /* __INTFFTW_INPLACE__ */
/*-----------------------------------------------------------------------------------*/
