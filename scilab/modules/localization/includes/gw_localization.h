/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_LOCALIZATION__
#define __GW_LOCALIZATION__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"

typedef int (*localization_interf) __PARAMS((char *fname,unsigned long fname_len));

/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_setlanguage) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getlanguage) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getlanguagealias) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getlanguagestable) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gettext) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_addtext) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_removetext) _PARAMS((char *fname,unsigned long fname_len));

int C2F(gw_localization)(void);

typedef struct table_struct {
	localization_interf f;    /** function **/
	char *name;      /** its name **/
} LocalizationTable;

#endif /*  __GW_LOCALIZATION__ */
/*-----------------------------------------------------------------------------------*/


