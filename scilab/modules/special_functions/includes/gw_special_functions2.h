/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SPECIAL_FUNCTIONS2__
#define __GW_SPECIAL_FUNCTIONS2__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "core_math.h"
#include "sciprint.h"

int C2F(gw_special_functions2)(void);

typedef int (*Special_Functions2_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Special_Functions2_Interf f;    /** function **/
  char *name;      /** its name **/
} SpecialFunctions2Table;

int sci_besselh(char *fname,unsigned long fname_len);
int sci_besseli(char *fname,unsigned long fname_len);
int sci_besselj(char *fname,unsigned long fname_len);
int sci_besselk(char *fname,unsigned long fname_len);
int sci_bessely(char *fname,unsigned long fname_len);
int sci_beta(char *fname,unsigned long fname_len);
int sci_legendre(char *fname,unsigned long fname_len);

#endif /*  __GW_SPECIAL_FUNCTIONS2__ */
/*-----------------------------------------------------------------------------------*/

