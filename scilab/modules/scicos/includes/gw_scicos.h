/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SCICOS__
#define __GW_SCICOS__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_scicos)(void);
/*--------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/ 
int C2F(sci_var2vec) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_vec2var) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_curblock) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getblocklabel) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scicos_debug) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scicos_debug_count) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sctree) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sci_tree2) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sci_tree3) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sci_tree4) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SCICOS__ */
/*--------------------------------------------------------------------------*/
