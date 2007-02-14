/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_OPTIM__
#define __GW_OPTIM__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Optim_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Optim_Interf f;    /** function **/
  char *name;      /** its name **/
} OptimTable;

int C2F(sci_optim) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_semidef) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fsolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sqrsolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_qld) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciquapro) _PARAMS((char *fname,unsigned long fname_len));

#endif /*  __GW_OPTIM__ */
/*-----------------------------------------------------------------------------------*/

