/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS2__
#define __GW_DIFFERENTIAL_EQUATIONS2__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

#ifdef _MSC_VER
#include "ExceptionMessage.h"
#endif

int C2F(gw_differential_equations2)(void);

typedef int (*Differential_Equations2_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations2_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations2Table;

extern int C2F(sciimpl) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(dassli) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(dasrti) _PARAMS((char *fname,unsigned long fname_len));


#endif  /* __GW_DIFFERENTIAL_EQUATIONS2__ */
/*-----------------------------------------------------------------------------------*/
