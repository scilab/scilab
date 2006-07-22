/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_TCLSCI__
#define __GW_TCLSCI__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "../../core/includes/machine.h"
#include "../../core/includes/stack-c.h"

#include "Errors.h"
#include "Warnings.h"
#include "Messages.h"


typedef int (*TCLSci_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  TCLSci_Interf f;    /** function **/
  char *name;      /** its name **/
} TCLSCITable;

#endif /*  __GW_TCLSCI__ */
/*-----------------------------------------------------------------------------------*/

