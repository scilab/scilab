/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_CACSD0__
#define __GW_CACSD0__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_cacsd0)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_gschur) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gspec) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ereduc) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fstair) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CACSD0__ */
/*--------------------------------------------------------------------------*/

