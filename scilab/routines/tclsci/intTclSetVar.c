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

		/* Efface valeur precedente */
		Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

		if ( !Tcl_SetVar(TCLinterp, VarName, VarValue, TCL_GLOBAL_ONLY) )
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
	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_matrix) )
	{
		char *VarName=NULL;
		char *VarNameWithIndice=NULL;
		int *paramoutINT=(int*)malloc(sizeof(int));
		int i=0;
		int j=0;
		int bTestTCL_SetVar=(int)(TRUE);
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);
		
		GetRhsVar(2,"d",&m1,&n1,&l1);

		if ( (m1==1) && (n1==1) )
		{
			/* just a scalar */
			char buffer[2048];

			sprintf(buffer,"%.10lf",*stk(l1));
			if (TCLinterp == NULL)
			{
				Scierror(999,"TCL_SetVar : Error TCLinterp not Initialize\r\n");
				return 0;
			}

			/* Efface valeur precedente */
			Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

			if (!Tcl_SetVar(TCLinterp,VarName,buffer,TCL_GLOBAL_ONLY))
			{
				bTestTCL_SetVar=(int)(FALSE);
			}
		}
		else 
		{
			/* Efface valeur precedente */
			Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

			for (j=1;j<n1+1;j++)
			{
				char buffer[2048];
				int lenJ=0;
				int lenI=0;

				sprintf(buffer,"%d",j);
				lenJ=strlen(buffer);

				for (i=1;i<m1+1;i++)
				{	
					int len=0;
					sprintf(buffer,"%d",i);
					lenI=strlen(buffer);
							
					len=strlen(VarName)+strlen("(,)")+lenI+lenJ+1;
					VarNameWithIndice=(char*)malloc(len*sizeof(char));
					sprintf(VarNameWithIndice,"%s(%d,%d)",VarName,j,i);
					sprintf(buffer,"%.10lf",*stk(l1++));

					if (TCLinterp == NULL)
					{
						Scierror(999,"TCL_SetVar : Error TCLinterp not Initialize\r\n");
						return 0;
					}

					if (!Tcl_SetVar(TCLinterp,VarNameWithIndice,buffer,0))
					{
						bTestTCL_SetVar=(int)(FALSE);
					}
					if (VarNameWithIndice){free(VarNameWithIndice);VarNameWithIndice=NULL;}
				}
            }
		}
		
		*paramoutINT=bTestTCL_SetVar;
		n1=1;
		CreateVarFromPtr(1, "b", &n1, &n1, &paramoutINT);
		LhsVar(1)=1;
		C2F(putlhsvar)();
		if (paramoutINT) {free(paramoutINT);paramoutINT=NULL;}
	}
	else
	{
		Scierror(999,"TCL_SetVar : Invalid Argument type.\r\n");
		return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
