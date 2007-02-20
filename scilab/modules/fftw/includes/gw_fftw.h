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

int C2F(gw_fftw)(void);

typedef int (*fftw_interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct {
  fftw_interf f;    /** function **/
  char *name;      /** its name **/
} intFFTWTable;

/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
int sci_fftw __PARAMS((char *fname,unsigned long fname_len));
int sci_fftw_flags __PARAMS((char *fname,unsigned long fname_len));
int sci_fftw_inplace __PARAMS((char *fname,unsigned long fname_len));
int sci_fftw_norm __PARAMS((char *fname,unsigned long fname_len));
int sci_pushfftw_plan __PARAMS((char *fname,unsigned long fname_len));
int sci_freefftw_plan __PARAMS((char *fname,unsigned long fname_len));
int sci_loadfftwlibrary __PARAMS((char *fname,unsigned long fname_len));
int sci_disposefftwlibrary __PARAMS((char *fname,unsigned long fname_len));
int sci_fftwlibraryisloaded __PARAMS((char *fname,unsigned long fname_len));

#endif /*  __GW_FFTW__ */
/*-----------------------------------------------------------------------------------*/
