#include "gw_scicos.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(sci_var2vec) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_vec2var) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_curblock) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getblocklabel) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_scicos_debug) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_scicos_debug_count) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sctree) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sci_tree2) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sci_tree3) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sci_tree4) _PARAMS((char *fname,unsigned long fname_len));
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
	#if _MSC_VER
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
