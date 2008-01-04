/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_LOCALIZATION__
#define __GW_LOCALIZATION__
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_localization)(void);
/*--------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/ 
int C2F(sci_setlanguage) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getlanguage) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getdefaultlanguage) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gettext) _PARAMS((char *fname,unsigned long fname_len));
/*int C2F(sci_getlanguagealias) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getlanguagestable) _PARAMS((char *fname,unsigned long fname_len));

int C2F(sci_addtext) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_removetext) _PARAMS((char *fname,unsigned long fname_len));*/
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_LOCALIZATION__ */
/*--------------------------------------------------------------------------*/


