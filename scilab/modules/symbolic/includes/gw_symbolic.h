/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SYMBOLIC__
#define __GW_SYMBOLIC__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/ 
int gw_symbolic(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sci_addf)(char *fname,unsigned long fname_len);
int C2F(sci_subf)(char *fname,unsigned long fname_len);
int C2F(sci_mulf)(char *fname,unsigned long fname_len);
int C2F(sci_ldivf)(char *fname,unsigned long fname_len);
int C2F(sci_rdivf)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SYMBOLIC__ */
/*--------------------------------------------------------------------------*/

