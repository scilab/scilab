/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclSetVar.h"
/*-----------------------------------------------------------------------------------*/
int SetVarStrings(char *VarName,char **Str,int m,int n);
int SetVarAString(char *VarName,char **Str);
int SetVarMatrix(char *VarName,int ptrValues,int m,int n);
int SetVarScalar(char *VarName,double VarValue);
/*-----------------------------------------------------------------------------------*/
int C2F(intTclSetVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));
	int TypeVar1=GetType(1);
	int TypeVar2=GetType(2);

	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings) )
	{
		char *VarName=NULL;
		char **Str=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);
		
		GetRhsVar(2,"S",&m1,&n1,&Str);

		/* Efface valeur precedente */
		Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

		if ( (m1==1) && (n1==1) )
		{
			*paramoutINT=SetVarAString(VarName,Str);
		}
		else
		{
			*paramoutINT=SetVarStrings(VarName,Str,m1,n1);
		}
		FreeRhsSVar(Str);
	}
	else
	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_matrix) )
	{
		#define COMPLEX 1
		char *VarName=NULL;
		int *header=NULL;
		int Cmplx;

		header = (int *) GetData(2);   Cmplx=header[3];
		if (Cmplx==COMPLEX)
		{
			Scierror(999,"doesn''t work with Complex");
			return 0;
		}
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);
		
		GetRhsVar(2,"d",&m1,&n1,&l1);
		

		if ( (m1==0) && (n1==0) )
		{
			Scierror(999,"[] doesn''t work with TCL");
			return 0;
		}

		if ( (m1==1) && (n1==1) )
		{
			*paramoutINT=SetVarScalar(VarName,(double)*stk(l1));
		}
		else 
		{
			*paramoutINT=SetVarMatrix(VarName,l1,m1,n1);
		}
	}
	else
	{
		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
		Scierror(999,TCL_ERROR24);
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
int SetVarMatrix(char *VarName,int ptrValues,int m,int n)
{
	int bOK=TRUE;
	int i=0,j=0;
	int k=0;

	double *MatrixDouble=(double*)MALLOC((m*n)*sizeof(double));

	/* Efface valeur precedente */
	Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);
	
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

			if (TCLinterp == NULL)
			{
				Scierror(999,TCL_ERROR23);
				return 0;
			}

			if (!Tcl_SetVar(TCLinterp,VarNameWithIndice,VarValueWithIndice,0))
			{
				bOK=(int)(FALSE);
			}
		}
	}
		if (MatrixDouble) {FREE(MatrixDouble);MatrixDouble=NULL;}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int SetVarScalar(char *VarName,double VarValue)
{
	int bOK=FALSE;

	/* just a scalar */
	char buffer[2048];

	sprintf(buffer,"%.10lf", VarValue);

	if (TCLinterp == NULL)
	{
		Scierror(999,TCL_ERROR23);
		return 0;
	}

	/* Efface valeur precedente */
	Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

	if (!Tcl_SetVar(TCLinterp,VarName,buffer,TCL_GLOBAL_ONLY))
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
int SetVarStrings(char *VarName,char **Str,int m,int n)
{
	int bOK=FALSE;

	int i=0,j=0;
	int l=0;
	int TestOnAllTcl_SetVar=TRUE;

	for (j=1;j<n+1;j++)
	{
		for (i=1;i<m+1;i++)
		{	
			char VarArrayName[1024];
			sprintf(VarArrayName,"%s(%d,%d)",VarName,i,j);
			if (Tcl_SetVar(TCLinterp, VarArrayName, Str[l++], TCL_GLOBAL_ONLY))
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
int SetVarAString(char *VarName,char **Str)
{
	int bOK=FALSE;

	if ( !Tcl_SetVar(TCLinterp, VarName, Str[0], TCL_GLOBAL_ONLY) )
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
