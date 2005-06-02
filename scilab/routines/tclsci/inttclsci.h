/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTTCLSCI__
#define __INTTCLSCI__

#ifdef WIN32
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "../machine.h"
#include "../machine.h"
#include "../stack-c.h"

#include "Errors.h"
#include "Warnings.h"
#include "Messages.h"


typedef int (*TCLSci_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  TCLSci_Interf f;    /** function **/
  char *name;      /** its name **/
} TCLSCITable;

int ReInitTCL(void);

#endif /*  __INTTCLSCI__ */
/*-----------------------------------------------------------------------------------*/

