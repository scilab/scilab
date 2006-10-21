/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_POLYNOMIALS__
#define __GW_POLYNOMIALS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Polynomials_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Polynomials_Interf f;    /** function **/
  char *name;      /** its name **/
} PolynomialsTable;

#endif /*  __GW_POLYNOMIALS__ */
/*-----------------------------------------------------------------------------------*/

