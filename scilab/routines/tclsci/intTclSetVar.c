/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclSetVar.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclSetVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int TypeVar1=GetType(1);
	int TypeVar2=GetType(2);

	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings) )
	{
		char *VarName=NULL,*VarValue=NULL;
		int *paramoutINT=(int*)malloc(sizeof(int));
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);
			
		GetRhsVar(2,"c",&m1,&n1,&l1);
		VarValue=cstk(l1);
		

		if ( !Tcl_SetVar(TCLinterp, VarName, VarValue, 0) )
		{
			*paramoutINT=(int)(FALSE);
		}
		else
		{
			*paramoutINT=(int)(TRUE);
		}
		n1=1;
		CreateVarFromPtr(1, "b", &n1, &n1, &paramoutINT);
		LhsVar(1)=1;
		C2F(putlhsvar)();

	}
	else
	{
		Scierror(999,"TCL_SetVar : Argument type must be character string \r\n");
		return 0;
	}

		
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
