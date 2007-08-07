/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __MATWINTOOLS__
#define __MATWINTOOLS__
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_wintools)(void);
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_winopen) _PARAMS((char *fname,unsigned long l));
int C2F(sci_winqueryreg) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddeopen) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddeclose) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddeexec) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddepoke) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddereq) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddeisopen) _PARAMS((char *fname,unsigned long l));
int C2F(sci_clipboard) _PARAMS((char *fname,unsigned long l));
int C2F(sci_hidetoolbar) _PARAMS((char *fname,unsigned long l));
int C2F(sci_toolbar) _PARAMS((char *fname,unsigned long l));
int C2F(sci_setlanguagemenu) _PARAMS((char *fname,unsigned long l));
int C2F(sci_consoledos) _PARAMS((char *fname,unsigned long l));
int C2F(sci_about) _PARAMS((char *fname,unsigned long l));
int C2F(sci_mcisendstring) _PARAMS((char *fname,unsigned long l));
int C2F(sci_oemtochar) _PARAMS((char *fname,unsigned long l));
int C2F(sci_chartooem) _PARAMS((char *fname,unsigned long l));
int C2F(sci_printsetupbox) _PARAMS((char *fname,unsigned long l));
int C2F(sci_toprint) _PARAMS((char *fname,unsigned long l));
int C2F(sci_settextcolor) _PARAMS((char *fname,unsigned long l));
int C2F(sci_settextbackgroundcolor) _PARAMS((char *fname,unsigned long l));
int C2F(sci_filesassociationbox) _PARAMS((char *fname,unsigned long l));
int C2F(sci_dos) _PARAMS((char *fname,unsigned long l));
int C2F(sci_findfileassociation) _PARAMS((char *fname,unsigned long l));
/*-----------------------------------------------------------------------------------*/ 

#endif /*  __MATWINTOOLS__ */
/*-----------------------------------------------------------------------------------*/

