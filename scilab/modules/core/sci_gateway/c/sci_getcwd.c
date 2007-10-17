/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "scicurdir.h"
#include "MALLOC.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getcwd) _PARAMS((char *fname,unsigned long fname_len))
{
	int ierr=0;
	int lpath=0;
	char *path=NULL;

	Rhs=Max(Rhs,0);

	CheckRhs(0,0);
	CheckLhs(0,1);

	C2F(scigetcwd)(&path,&lpath,&ierr);
	
	if (ierr)
	{
		error_scilab(998,_("Internal Error."));
		return 0;
	}
	else
	{
		static int n1,m1;
		n1=1;
		CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(path), &m1),&n1,&path);
		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();	
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
