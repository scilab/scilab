/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_TCLSCI__
#define __GW_TCLSCI__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int gw_tclsci(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sci_TCL_DoOneEvent) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_EvalFile) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_EvalStr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_GetVar) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_SetVar) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_opentk) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_set) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_get) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_gcf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_scf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_GetVersion) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_UnsetVar) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_ExistVar) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_UpVar) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_DeleteInterp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_CreateSlave) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_ExistInterp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_TCL_ExistArray) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_TCLSCI__ */
/*--------------------------------------------------------------------------*/

