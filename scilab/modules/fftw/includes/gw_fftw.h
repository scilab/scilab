/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __GW_FFTW__
#define __GW_FFTW__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "sci_get_fftw_wisdom.h"
#include "sci_fftw_flags.h"
#include "sci_loadfftwlibrary.h"
#include "sci_set_fftw_wisdom.h"
#include "sci_fftw_forget_wisdom.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_fftw)(void);
/*--------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/
int sci_fftw __PARAMS((char *fname,unsigned long fname_len));
int sci_disposefftwlibrary __PARAMS((char *fname,unsigned long fname_len));
int sci_fftwlibraryisloaded __PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_FFTW__ */
/*--------------------------------------------------------------------------*/
