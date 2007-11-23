/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_BOOLEAN__
#define __GW_BOOLEAN__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_boolean)(void);
/*--------------------------------------------------------------------------*/
int C2F(intor) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intand) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_find) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_bool2s) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_BOOLEAN__ */
/*--------------------------------------------------------------------------*/
