/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_INCREMENTAL_LINK__
#define __GW_INCREMENTAL_LINK__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/
int gw_dynamic_link(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
/*--------------------------------------------------------------------------*/ 
int C2F(sci_getdynlibext) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_addinter) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fort) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_call) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_link) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ulink) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_c_link) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_INCREMENTAL_LINK__ */
/*--------------------------------------------------------------------------*/ 
