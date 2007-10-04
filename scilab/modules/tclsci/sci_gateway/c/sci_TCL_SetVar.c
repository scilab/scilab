/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "error_scilab.h"
#include "setvar.h"
/*-----------------------------------------------------------------------------------*/
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
		error_scilab(999,"%s : Error main TCL interpreter not initialized.",fname);
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
				error_scilab(999,"%s: No such slave interpreter.",fname);
				return 0;
			}
		}
		else
		{
			error_scilab(999,"%s : Argument type must be character string.",fname);
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
		if (Str) for (i = 0; i<m1*n1 ;i++)
		{
			if (Str[i]) 
			{
				FREE(Str[i]);
				Str[i]=NULL;
			}
		}
		if (Str) {FREE(Str); Str=NULL;}
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
			error_scilab(999,"doesn't work with Complex");
			return 0;
		}
		
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);
		
		GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);

		if ( (m1==0) && (n1==0) )
		{
			error_scilab(999,"[] doesn't work with Tcl/Tk.");
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
		error_scilab(999,"%s: Invalid argument type.",fname);
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
/*-----------------------------------------------------------------------------------*/
