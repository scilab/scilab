/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_UpVar) _PARAMS((char *fname,unsigned long l))
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
		
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		sourceName=cstk(l1);
			
		GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
		destName=cstk(l1);

		if (TCLinterp == NULL)
		{
			error_scilab(999,"tclsci_error_12",fname);
			return 0;
		}

		if (Rhs==3)
		{
			/* three arguments given - get a pointer on the slave interpreter */
			if (GetType(3) == sci_strings)
			{
				GetRhsVar(3,STRING_DATATYPE,&m2,&n2,&l2);
				TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
				if (TCLinterpreter==NULL)
				{
					error_scilab(999,"tclsci_error_16",fname);
					return 0;
				}
			}
			else
			{
				error_scilab(999,"tclsci_error_15",fname);
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
		CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}

	}
	else
	{
		error_scilab(999,"tclsci_error_15",fname);
		return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
