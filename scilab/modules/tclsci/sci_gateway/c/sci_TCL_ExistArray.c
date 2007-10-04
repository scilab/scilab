/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "TCL_ArrayExist.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_ExistArray) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	static int l2,n2,m2;

	int ValRet=0;

	Tcl_Interp *TCLinterpreter=NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);
	
	if (GetType(1) == sci_strings)
	{
		char *VarName=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);

		if (TCLinterp == NULL)
		{
			error_scilab(999,"%s : Error main TCL interpreter not initialized.",fname);
			return 0;
		}
		
		if (Rhs==2)
		{
			/* two arguments given - get a pointer on the slave interpreter */
			if (GetType(2) == sci_strings)
			{
				GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
				TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
				if (TCLinterpreter==NULL)
				{
					error_scilab(999,"%s: No such slave interpreter.",fname);
					return 0;
				}
			}
			else
			{
				error_scilab(999,"%s : Argument type must be character string.");
				return 0;
			}
		}
		else
		{
			/* only one argument given - use the main interpreter */
			TCLinterpreter=TCLinterp;
		}

		ValRet=TCL_ArrayExist(TCLinterpreter,VarName);
		
		n1=1;
		if ( ValRet )
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		else
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}
	
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		error_scilab(999,"%s : Argument type must be character string.",fname);
		return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
