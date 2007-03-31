/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_FFTW__
#define __GW_FFTW__
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_fftw)(void);
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
/*-----------------------------------------------------------------------------------*/
#endif /*  __GW_FFTW__ */
/*-----------------------------------------------------------------------------------*/
