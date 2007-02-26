/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS1__
#define __GW_DIFFERENTIAL_EQUATIONS1__

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

int C2F(gw_differential_equations1)(void);

typedef int (*Differential_Equations1_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations1_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations1Table;

int C2F(sci_ode) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_intg) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_feval) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_bvode) _PARAMS((char *fname,unsigned long fname_len));

#endif  /* __GW_DIFFERENTIAL_EQUATIONS1__ */
/*-----------------------------------------------------------------------------------*/
