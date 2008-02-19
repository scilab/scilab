/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SYMBOLIC__
#define __GW_SYMBOLIC__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int gw_symbolic(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sci_addf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_subf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_mulf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ldivf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rdivf) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SYMBOLIC__ */
/*--------------------------------------------------------------------------*/

