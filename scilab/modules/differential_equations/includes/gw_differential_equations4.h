/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS4__
#define __GW_DIFFERENTIAL_EQUATIONS4__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

int C2F(gw_differential_equations4)(void);

typedef int (*Differential_Equations4_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations4_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations4Table;

int C2F(sci_intg) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_int2d) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_int3d) _PARAMS((char *fname,unsigned long fname_len));

#endif  /* __GW_DIFFERENTIAL_EQUATIONS4__ */
/*-----------------------------------------------------------------------------------*/
