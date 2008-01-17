/*--------------------------------------------------------------------------*/
/**
  * @INRIA 2008
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_graphic_export.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
typedef int (*Graphic_Export_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct
{
  Graphic_Export_Interf f;    /** function **/
  char *name;      /** its name **/
} Graphic_ExportTable;
/*--------------------------------------------------------------------------*/
/*
static Graphic_ExportTable Tab[]=
{
{C2F(sci_skeleton_c),"skeleton_c"},
};
*/
/*--------------------------------------------------------------------------*/
int C2F(gw_graphic_export)()
{  
	Rhs = Max(0, Rhs);
/*
#ifdef _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
*/
	return 0;
}
/*--------------------------------------------------------------------------*/
