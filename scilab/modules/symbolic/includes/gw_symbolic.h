/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SYMBOLIC__
#define __GW_SYMBOLIC__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

int C2F(gw_symbolic)(void);

typedef int (*Symbolic_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Symbolic_Interf f;    /** function **/
  char *name;      /** its name **/
} SymbolicTable;


int C2F(sci_addf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_subf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_mulf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ldivf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rdivf) _PARAMS((char *fname,unsigned long fname_len));

#endif /*  __GW_SYMBOLIC__ */
/*-----------------------------------------------------------------------------------*/

