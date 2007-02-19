/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS5__
#define __GW_DIFFERENTIAL_EQUATIONS5__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

int C2F(gw_differential_equations5)(void);

typedef int (*Differential_Equations5_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations5_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations5Table;

static int C2F(sci_feval) _PARAMS((char *fname,unsigned long fname_len));


#endif  /* __GW_DIFFERENTIAL_EQUATIONS5__ */
/*-----------------------------------------------------------------------------------*/
