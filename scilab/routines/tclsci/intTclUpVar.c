/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclUpVar.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclUpVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int TypeVar1=GetType(1);
	int TypeVar2=GetType(2);

	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings) )
	{
		char *sourceName=NULL,*destName=NULL;
		int *paramoutINT=(int*)malloc(sizeof(int));
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		sourceName=cstk(l1);
			
		GetRhsVar(2,"c",&m1,&n1,&l1);
		destName=cstk(l1);

		if ( Tcl_UpVar(TCLinterp,"#0", sourceName, destName, TCL_GLOBAL_ONLY) == TCL_ERROR )
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
		if (paramoutINT) {free(paramoutINT);paramoutINT=NULL;}

	}
	else
	{
		Scierror(999,"TCL_UpVar : Invalid Argument type.\r\n");
		return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
