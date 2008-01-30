/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_CSCICOS__
#define __GW_CSCICOS__
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_cscicos)(void);
/*--------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/ 
int sci_scicos_time _PARAMS((char *fname,unsigned long fname_len));
int sci_duplicate _PARAMS((char *fname,unsigned long fname_len));
int sci_diffobjs _PARAMS((char *fname,unsigned long fname_len));
int sci_pointer_xproperty _PARAMS((char *fname,unsigned long fname_len));
int sci_phase_simulation _PARAMS((char *fname,unsigned long fname_len));
int sci_set_xproperty _PARAMS((char *fname,unsigned long fname_len));
int sci_set_blockerror _PARAMS((char *fname,unsigned long fname_len));
int sci_ctree2 _PARAMS((char *fname,unsigned long fname_len));
int sci_ctree3 _PARAMS((char *fname,unsigned long fname_len));
int sci_ctree4 _PARAMS((char *fname,unsigned long fname_len));
int sci_scicosim _PARAMS((char *fname,unsigned long fname_len));
int sci_getscicosvars _PARAMS((char *fname,unsigned long fname_len));
int sci_curblockc _PARAMS((char *fname,unsigned long fname_len));
int sci_buildouttb _PARAMS((char *fname,unsigned long fname_len));
int sci_permutobj _PARAMS((char *fname,unsigned long fname_len));
int sci_xstringb2 _PARAMS((char *fname,unsigned long fname_len));
int sci_end_scicosim _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CSCICOS__ */
/*--------------------------------------------------------------------------*/
