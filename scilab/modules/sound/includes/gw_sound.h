/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SOUND__
#define __GW_SOUND__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_sound)(void);
/*--------------------------------------------------------------------------*/ 
int sci_Beep __PARAMS((char *fname,unsigned long fname_len));
int sci_Playsound __PARAMS((char *fname,unsigned long fname_len));
int sci_savewave __PARAMS((char *fname,unsigned long fname_len));
int sci_loadwave __PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SOUND__ */
/*--------------------------------------------------------------------------*/
