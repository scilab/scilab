/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DATA_STRUCTURES1__
#define __GW_DATA_STRUCTURES1__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

int C2F(gwdatastructures1)(void);

typedef int (*Data_structures1_Interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct 
{
	Data_structures1_Interf f;    /** function **/
	char *name;      /** its name **/
} DataStructures1Table;


int C2F(sci_glist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scilist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lsslist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rlist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scitlist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lstcat) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scimlist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_definedfields) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lstsize) _PARAMS((char *fname,unsigned long fname_len));

#endif /*  __GW_DATA_STRUCTURES1__ */
/*-----------------------------------------------------------------------------------*/

