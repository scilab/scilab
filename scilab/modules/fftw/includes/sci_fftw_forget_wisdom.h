/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __INTFFTW_FORGET_WISDOM__
#define __INTFFTW_FORGET_WISDOM__

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int sci_fftw_forget_wisdom __PARAMS((char *fname,unsigned long fname_len));

#endif /* __INTFFTW__ */
/*--------------------------------------------------------------------------*/
