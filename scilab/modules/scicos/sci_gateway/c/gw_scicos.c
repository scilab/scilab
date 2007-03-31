/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_scicos.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/ 
typedef int (*scicos_interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct {
	scicos_interf f;    /** function **/
	char *name;      /** its name **/
} intscicosTable;
/*-----------------------------------------------------------------------------------*/ 
static intscicosTable Tab[]={
{C2F(sci_var2vec),"var2vec"},
{C2F(sci_vec2var),"vec2var"},
{C2F(sci_curblock),"curblock"},
{C2F(sci_getblocklabel),"getblocklabel"},
{C2F(sci_scicos_debug),"scicos_debug"},
{C2F(sci_scicos_debug_count),"scicos_debug_count"},
{C2F(sci_sctree),"sctree"},
{C2F(sci_sci_tree2),"sci_tree2"},
{C2F(sci_sci_tree3),"sci_tree3"},
{C2F(sci_sci_tree4),"sci_tree4"}
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_scicos)()
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
/*-----------------------------------------------------------------------------------*/ 
