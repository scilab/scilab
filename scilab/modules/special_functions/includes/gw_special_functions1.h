/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SPECIAL_FUNCTIONS__
#define __GW_SPECIAL_FUNCTIONS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "core_math.h"
#include "sciprint.h"

int C2F(gw_special_functions1)(void);

typedef int (*Special_Functions_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Special_Functions_Interf f;    /** function **/
  char *name;      /** its name **/
} SpecialFunctionsTable;

int C2F(sci_oldbesseli) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_oldbesselj) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_oldbesselk) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_oldbessely) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gamma) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lgamma) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_dlgamma) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_calerf) _PARAMS((char *fname,unsigned long fname_len));

#endif /*  __GW_SPECIAL_FUNCTIONS__ */
/*-----------------------------------------------------------------------------------*/

