/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS3__
#define __GW_DIFFERENTIAL_EQUATIONS3__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

int C2F(gw_differential_equations3)(void);

typedef int (*Differential_Equations3_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations3_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations3Table;

static int C2F(sci_odc) _PARAMS((char *fname,unsigned long l));


#endif  /* __GW_DIFFERENTIAL_EQUATIONS3__ */
/*-----------------------------------------------------------------------------------*/
