/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclExistVar.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclExistVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int TypeVar1=GetType(1);

	CheckRhs(1,1);
	CheckLhs(1,1);
	
	if (TypeVar1 == sci_strings)
	{
		int *paramoutINT=(int*)malloc(sizeof(int));

		char *VarName=NULL;
		char *RetStr=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);

		if (TCLinterp == NULL)
		{
			Scierror(999,"TCL_ExistVar : Error TCLinterp not Initialize\r\n");
			return 0;
		}

		if ( Tcl_GetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY) )
		{
			*paramoutINT=(int)(TRUE);
		}
		else
		{
			*paramoutINT=(int)(FALSE);
		}

		n1=1;
		CreateVarFromPtr(1, "b", &n1, &n1, &paramoutINT);
		LhsVar(1)=1;
		C2F(putlhsvar)();
		if (paramoutINT) {free(paramoutINT);paramoutINT=NULL;}

	}
	else
	{
		 Scierror(999,"TCL_ExistVar : Argument type must be character string \r\n");
		 return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
