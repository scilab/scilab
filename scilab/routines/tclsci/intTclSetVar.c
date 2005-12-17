/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclSetVar.h"
/*-----------------------------------------------------------------------------------*/
int SetVarStrings(Tcl_Interp *TCLinterpreter,char *VarName,char **Str,int m,int n);
int SetVarAString(Tcl_Interp *TCLinterpreter,char *VarName,char **Str);
int SetVarMatrix(Tcl_Interp *TCLinterpreter,char *VarName,int ptrValues,int m,int n);
int SetVarScalar(Tcl_Interp *TCLinterpreter,char *VarName,double VarValue);
/*-----------------------------------------------------------------------------------*/
int C2F(intTclSetVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	static int l2,n2,m2;
	int *paramoutINT=(int*)MALLOC(sizeof(int));
	Tcl_Interp *TCLinterpreter=NULL;

	CheckRhs(2,3);
	CheckLhs(0,1);

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

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		char *VarName=NULL;
		char **Str=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);
		
		GetRhsVar(2,"S",&m1,&n1,&Str);

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
		FreeRhsSVar(Str);
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
			Scierror(999,"doesn't work with Complex");
			return 0;
		}
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);
		
		GetRhsVar(2,"d",&m1,&n1,&l1);

		if ( (m1==0) && (n1==0) )
		{
			Scierror(999,"[] doesn't work with TCL");
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
		Scierror(999,TCL_ERROR24,fname);
		return 0;
	}
	
	m1=1;
	n1=1;
	l1=0;
	CreateVar(Rhs+1,"b",  &m1, &n1, &l1);
	*istk(l1)=*paramoutINT;

	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int SetVarMatrix(Tcl_Interp *TCLinterpreter,char *VarName,int ptrValues,int m,int n)
{
	int bOK=TRUE;
	int i=0,j=0;
	int k=0;

	double *MatrixDouble=(double*)MALLOC((m*n)*sizeof(double));

	/* Efface valeur precedente */
	Tcl_UnsetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);
	
	for (i=0;i<m*n;i++)
	{
		MatrixDouble[i]=*stk(ptrValues++);
	}

	k=0;
	for (i=1;i<m+1;i++)
	{
		char VarValueWithIndice[2048];
		char VarNameWithIndice[2048];
		
		for (j=1;j<n+1;j++)
		{	
			int bOKsprintf1=0;
			int bOKsprintf2=0;

			bOKsprintf1=sprintf(VarNameWithIndice,"%s(%d,%d)",VarName,i,j);
			bOKsprintf2=sprintf(VarValueWithIndice,"%.10lf",MatrixDouble[(i-1)+m*(j-1)]);

			if ( (bOKsprintf1 == -1) || (bOKsprintf2 == -1) )
			{
				Scierror(999,"Variable too long");
				return 0;
			}

			if (TCLinterpreter == NULL)
			{
				Scierror(999,TCL_ERROR23);
				return 0;
			}

			if (!Tcl_SetVar(TCLinterpreter,VarNameWithIndice,VarValueWithIndice,0))
			{
				bOK=(int)(FALSE);
			}
		}
	}
		if (MatrixDouble) {FREE(MatrixDouble);MatrixDouble=NULL;}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int SetVarScalar(Tcl_Interp *TCLinterpreter,char *VarName,double VarValue)
{
	int bOK=FALSE;

	/* just a scalar */
	char buffer[2048];

	sprintf(buffer,"%.10lf", VarValue);

	if (TCLinterpreter == NULL)
	{
		Scierror(999,TCL_ERROR23);
		return 0;
	}

	/* Efface valeur precedente */
	Tcl_UnsetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);

	if (!Tcl_SetVar(TCLinterpreter,VarName,buffer,TCL_GLOBAL_ONLY))
	{
		bOK=(int)(FALSE);
	}
	else
	{
		bOK=(int)(TRUE);
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int SetVarStrings(Tcl_Interp *TCLinterpreter,char *VarName,char **Str,int m,int n)
{
	int bOK=FALSE;

	int i=0,j=0;
	int l=0;
	int TestOnAllTcl_SetVar=TRUE;

	if (TCLinterpreter == NULL)
	{
		Scierror(999,TCL_ERROR23);
		return 0;
	}

	for (j=1;j<n+1;j++)
	{
		for (i=1;i<m+1;i++)
		{	
			char VarArrayName[1024];
			sprintf(VarArrayName,"%s(%d,%d)",VarName,i,j);
			if (Tcl_SetVar(TCLinterpreter, VarArrayName, Str[l++], TCL_GLOBAL_ONLY))
			{
				if (TestOnAllTcl_SetVar) TestOnAllTcl_SetVar=TRUE;
			}
			else
			{
				TestOnAllTcl_SetVar=FALSE;
			}
		}
	}
	bOK=(int)(TestOnAllTcl_SetVar);
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int SetVarAString(Tcl_Interp *TCLinterpreter,char *VarName,char **Str)
{
	int bOK=FALSE;

	if (TCLinterpreter == NULL)
	{
		Scierror(999,TCL_ERROR23);
		return 0;
	}

	if ( !Tcl_SetVar(TCLinterpreter, VarName, Str[0], TCL_GLOBAL_ONLY) )
	{
		bOK=(int)(FALSE);
	}
	else
	{
		bOK=(int)(TRUE);
	}
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
