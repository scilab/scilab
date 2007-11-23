/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __INTFFTW_FLAGS__
#define __INTFFTW_FLAGS__

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int sci_fftw_flags __PARAMS((char *fname,unsigned long fname_len));

#endif /* __INTFFTW_FLAGS__ */
/*--------------------------------------------------------------------------*/
