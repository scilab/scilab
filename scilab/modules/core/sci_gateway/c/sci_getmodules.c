/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "getmodules.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_getmodules) _PARAMS((char *fname,unsigned long fname_len))
{
	struct MODULESLIST *Modules=NULL;

	int ncol=0;	
	int nrow=0;

	Modules=getmodules();
	if (!Modules)
	{
		error_scilab(999,"Warning : Problems with %s.",fname);
		return 0;
	}

	ncol=1;
	nrow=Modules->numberofModules;

	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &nrow, &ncol, Modules->ModuleList);
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();

	/* je ne libere pas Modules pour accelerer le prochain appel à getmodule */
	/* liberé dans sciquit.c */

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
