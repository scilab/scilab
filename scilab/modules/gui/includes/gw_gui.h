/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_GUI__
#define __GW_GUI__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Gui_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Gui_Interf f;    /** function **/
  char *name;      /** its name **/
} GuiTable;

#endif /*  __GW_GUI__ */
/*-----------------------------------------------------------------------------------*/

