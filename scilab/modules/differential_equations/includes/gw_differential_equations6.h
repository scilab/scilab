/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS6__
#define __GW_DIFFERENTIAL_EQUATIONS6__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

int C2F(gw_differential_equations6)(void);

typedef int (*Differential_Equations6_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations6_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations6Table;

extern int C2F(scibvode) __PARAMS((char *fname,unsigned long fname_len));

#endif  /* __GW_DIFFERENTIAL_EQUATIONS6__ */
/*-----------------------------------------------------------------------------------*/
