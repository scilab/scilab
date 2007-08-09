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
int C2F(sci_ddeopen) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddeclose) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddeexec) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddepoke) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddereq) _PARAMS((char *fname,unsigned long l));
int C2F(sci_ddeisopen) _PARAMS((char *fname,unsigned long l));
int C2F(sci_setlanguagemenu) _PARAMS((char *fname,unsigned long l));
int C2F(sci_settextcolor) _PARAMS((char *fname,unsigned long l));
int C2F(sci_settextbackgroundcolor) _PARAMS((char *fname,unsigned long l));
int C2F(sci_filesassociationbox) _PARAMS((char *fname,unsigned long l));
/*-----------------------------------------------------------------------------------*/ 

#endif /*  __MATWINTOOLS__ */
/*-----------------------------------------------------------------------------------*/

