/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SIGNAL__
#define __GW_SIGNAL__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_signal)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_ffir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fft) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_corr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fiir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rpem) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_amell) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_delip) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_remez) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_syredi) _PARAMS((char *fname,unsigned long fname_len));
#endif /*  __GW_SIGNAL__ */
/*--------------------------------------------------------------------------*/

