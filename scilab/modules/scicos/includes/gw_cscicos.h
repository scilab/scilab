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
int inttimescicos _PARAMS((char *fname,unsigned long l));
int intduplicate _PARAMS((char *fname,unsigned long l));
int intdiffobjs _PARAMS((char *fname,unsigned long l));
int intxproperty _PARAMS((char *fname,unsigned long l));
int intphasesim _PARAMS((char *fname,unsigned long l));
int intsetxproperty _PARAMS((char *fname,unsigned long l));
int intcpass2 _PARAMS((char *fname,unsigned long l));
int intsetblockerror _PARAMS((char *fname,unsigned long l));
int inttree2 _PARAMS((char *fname,unsigned long l));
int inttree3 _PARAMS((char *fname,unsigned long l));
int inttree4 _PARAMS((char *fname,unsigned long l));
int intscicosimc _PARAMS((char *fname, unsigned long l));
int intgetscicosvarsc _PARAMS((char *fname, unsigned long l));
int intcurblkc _PARAMS((char *fname, unsigned long l));
int intbuildouttb _PARAMS((char *fname));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CSCICOS__ */
/*--------------------------------------------------------------------------*/
