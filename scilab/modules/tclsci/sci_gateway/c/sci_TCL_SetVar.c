/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "setvar.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_TCL_SetVar) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	static int l2,n2,m2;
	int *paramoutINT=(int*)MALLOC(sizeof(int));
	Tcl_Interp *TCLinterpreter=NULL;

	CheckRhs(2,3);
	CheckLhs(0,1);

	if (TCLinterp == NULL)
	{
		Scierror(999,_("%s : Error main TCL interpreter not initialized.\n"),fname);
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
				Scierror(999,_("%s: No such slave interpreter.\n"),fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s : Argument type must be character string.\n"),fname);
			return 0;
		}
	}
	else
	{
		/* only two arguments given - use the main interpreter */
		TCLinterpreter=TCLinterp;
	}

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		char *VarName=NULL;
		char **Str=NULL;
		int i=0;
		
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);
		
		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

		/* Efface valeur precedente */
		Tcl_UnsetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);

		if ( (m1==1) && (n1==1) )
		{
			*paramoutINT=SetVarAString(TCLinterpreter,VarName,Str);
		}
		else
		{
			*paramoutINT=SetVarStrings(TCLinterpreter,VarName,Str,m1,n1);
		}
		freeArrayOfString(Str,m1*n1);
	}
	else
	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_matrix) )
	{
		#define COMPLEX 1
		char *VarName=NULL;
		int *header=NULL;
		int Cmplx;

		header = (int *) GetData(2);   Cmplx=header[3];
		if (Cmplx==COMPLEX)
		{
			Scierror(999,_("doesn't work with Complex\n"));
			return 0;
		}
		
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);
		
		GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);

		if ( (m1==0) && (n1==0) )
		{
			Scierror(999,_("[] doesn't work with Tcl/Tk.\n"));
			return 0;
		}

		if ( (m1==1) && (n1==1) )
		{
			*paramoutINT=SetVarScalar(TCLinterpreter,VarName,(double)*stk(l1));
		}
		else 
		{
			*paramoutINT=SetVarMatrix(TCLinterpreter,VarName,l1,m1,n1);
		}
	}
	else
	{
		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
		Scierror(999,_("%s: Invalid argument type.\n"),fname);
		return 0;
	}
	
	m1=1;
	n1=1;
	l1=0;
	CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE,  &m1, &n1, &l1);
	*istk(l1)=*paramoutINT;

	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	
	
	return 0;
}
/*--------------------------------------------------------------------------*/
