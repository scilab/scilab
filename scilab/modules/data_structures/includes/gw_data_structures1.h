/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_DATA_STRUCTURES1__
#define __GW_DATA_STRUCTURES1__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gwdatastructures1)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_glist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scilist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lsslist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rlist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scitlist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lstcat) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scimlist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_definedfields) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lstsize) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_DATA_STRUCTURES1__ */
/*--------------------------------------------------------------------------*/

