/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_TIME__
#define __GW_TIME__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_time)(void);
/*--------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/ 
int C2F(sci_getdate) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_calendar) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_timer) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sleep) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_xpause) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_realtimeinit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_realtime) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_TIME__ */
/*--------------------------------------------------------------------------*/
