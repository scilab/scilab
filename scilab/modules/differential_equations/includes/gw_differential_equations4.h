/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS4__
#define __GW_DIFFERENTIAL_EQUATIONS4__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_differential_equations4)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_intg) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_int2d) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_int3d) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif  /* __GW_DIFFERENTIAL_EQUATIONS4__ */
/*--------------------------------------------------------------------------*/
