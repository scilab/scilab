/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_FFTW__
#define __GW_FFTW__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"

typedef int (*fftw_interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct {
  fftw_interf f;    /** function **/
  char *name;      /** its name **/
} intFFTWTable;

#endif /*  __GW_FFTW__ */
/*-----------------------------------------------------------------------------------*/
