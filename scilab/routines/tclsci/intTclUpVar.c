/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclUpVar.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclUpVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	static int l2,n2,m2;
	
	Tcl_Interp *TCLinterpreter=NULL;

	CheckRhs(2,3);
	CheckLhs(0,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		char *sourceName=NULL,*destName=NULL;
		int *paramoutINT=(int*)MALLOC(sizeof(int));
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		sourceName=cstk(l1);
			
		GetRhsVar(2,"c",&m1,&n1,&l1);
		destName=cstk(l1);

		if (TCLinterp == NULL)
		{
			Scierror(999,TCL_ERROR13,fname);
			return 0;
		}

		if (Rhs==3)
		{
			/* three arguments given - get a pointer on the slave interpreter */
			if (GetType(3) == sci_strings)
			{
				GetRhsVar(3,"c",&m2,&n2,&l2)
				TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
				if (TCLinterpreter==NULL)
				{
					Scierror(999,TCL_ERROR17,fname);
					return 0;
				}
			}
			else
			{
				 Scierror(999,TCL_ERROR14,fname);
				 return 0;
			}
		}
		else
		{
			/* only two arguments given - use the main interpreter */
			TCLinterpreter=TCLinterp;
		}

		if ( Tcl_UpVar(TCLinterpreter,"#0", sourceName, destName, TCL_GLOBAL_ONLY) == TCL_ERROR )
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
		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}

	}
	else
	{
		Scierror(999,TCL_ERROR14,fname);
		return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
