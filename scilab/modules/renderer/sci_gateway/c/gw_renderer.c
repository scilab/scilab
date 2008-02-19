/*--------------------------------------------------------------------------*/
/**
  * @INRIA 2007
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_renderer.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
typedef int (*Renderer_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct
{
  Renderer_Interf f;    /** function **/
  char *name;      /** its name **/
} RendererTable;
/*--------------------------------------------------------------------------*/
static RendererTable Tab[]=
{
//{C2F(sci_skeleton_c),"skeleton_c"},
};
/*--------------------------------------------------------------------------*/
int gw_renderer(void)
{  
	Rhs = Max(0, Rhs);

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
	return 0;
}
/*--------------------------------------------------------------------------*/
