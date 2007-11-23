/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_OPTIM__
#define __GW_OPTIM__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_optim)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_optim) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_semidef) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fsolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sqrsolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_qld) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciquapro) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_quapro) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intqld)_PARAMS((char *fname,unsigned long fname_len));
int C2F(intlsqrsolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(scioptim) _PARAMS((char *fname,unsigned long fname_len));
int C2F(scisolv)  _PARAMS((char *fname,unsigned long fname_len));
int C2F(scisemidef)_PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_OPTIM__ */
/*--------------------------------------------------------------------------*/

