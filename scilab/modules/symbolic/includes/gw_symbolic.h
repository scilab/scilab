/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SYMBOLIC__
#define __GW_SYMBOLIC__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_symbolic.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/ 
SYMBOLIC_IMPEXP int gw_symbolic(void);
/*--------------------------------------------------------------------------*/ 
SYMBOLIC_IMPEXP int sci_addf(char *fname,unsigned long fname_len);
SYMBOLIC_IMPEXP int sci_subf(char *fname,unsigned long fname_len);
SYMBOLIC_IMPEXP int sci_mulf(char *fname,unsigned long fname_len);
SYMBOLIC_IMPEXP int sci_ldivf(char *fname,unsigned long fname_len);
SYMBOLIC_IMPEXP int sci_rdivf(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SYMBOLIC__ */
/*--------------------------------------------------------------------------*/

