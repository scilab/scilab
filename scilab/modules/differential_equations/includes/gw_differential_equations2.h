/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS2__
#define __GW_DIFFERENTIAL_EQUATIONS2__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_differential_equations2)(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sciimpl) _PARAMS((char *fname,unsigned long fname_len));
int C2F(dassli) _PARAMS((char *fname,unsigned long fname_len));
int C2F(dasrti) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif  /* __GW_DIFFERENTIAL_EQUATIONS2__ */
/*--------------------------------------------------------------------------*/
