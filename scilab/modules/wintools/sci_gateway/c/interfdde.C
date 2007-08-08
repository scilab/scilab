/* Allan CORNET */
/* INRIA 2004 */
#include "interfdde.h"
#include "win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
#define MSG_ERROR14 "parameter(s) type incorrect.\r\n"
#define MSG_ERROR15 "2 parameters required.\r\n"
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEopen _PARAMS((char *fname))
{
	static int l1,n1,m1;
	Rhs=Max(0,Rhs);
	CheckRhs(2,2);
	CheckLhs(0,1);

	if (Rhs == 2)
	{
		int TypeVar1=GetType(1);
		int TypeVar2=GetType(2);

		if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings) )
		{
			char *param1=NULL,*param2=NULL;
			
			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		    param1=cstk(l1);

			GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
		    param2=cstk(l1);

			if ( DDEOpenConnection(param1,param2) )
			{
				int *paramoutINT=(int*)MALLOC(sizeof(int));
				*paramoutINT=(int)(TRUE);
				n1=1;
				CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
				LhsVar(1)=Rhs+1;
				FREE(paramoutINT);
			}
			else
			{
				int *paramoutINT=(int*)MALLOC(sizeof(int));
				*paramoutINT=(int)(FALSE);
				n1=1;
				CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
				LhsVar(1)=Rhs+1;
				FREE(paramoutINT);
			}
		}
		else 
		{
			Scierror(999,MSG_ERROR14);
		}
	}
	else
	{
		Scierror(999,MSG_ERROR15);
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEclose _PARAMS((char *fname))
{
	static int n1;
	Rhs=Max(0,Rhs);
	CheckRhs(0,0);
	CheckLhs(0,1);

	if ( DDECloseConnection() )
	{
		int *paramoutINT=(int*)MALLOC(sizeof(int));
		*paramoutINT=(int)TRUE;
		n1=1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
		LhsVar(1)=Rhs+1;
		FREE(paramoutINT);
	}
	else
	{
		int *paramoutINT=(int*)MALLOC(sizeof(int));
		*paramoutINT=(int)FALSE;
		n1=1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
		LhsVar(1)=Rhs+1;
		FREE(paramoutINT);
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEexec _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *param2=NULL;

	int TypeVar1=GetType(1);

	Rhs=Max(0,Rhs);
	CheckRhs(1,2);
	CheckLhs(0,1);

	if (Rhs == 2)
	{
		int TypeVar2=GetType(2);
		if (TypeVar2 == sci_matrix)
		{
			GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
			if ( (m1 == 1) && (n1 == 1) )
			{
				param2=istk(l1);
			}
			else
			{
				Scierror(999,MSG_ERROR14);
				return 0;
			}
		}
		else
		{
			Scierror(999,MSG_ERROR14);
			return 0;
		}
	}

	if (Rhs == 1)
	{
		param2=MALLOC(sizeof(int));
		*param2=-1;
	}

	if (TypeVar1 == sci_strings)
	{
		char *param1=NULL;
			
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		if ( DDEExecute(param1,*param2) )
		{
			int *paramoutINT=(int*)MALLOC(sizeof(int));
			*paramoutINT=(int)TRUE;
			n1=1;
			CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
			LhsVar(1)=Rhs+1;
			FREE(paramoutINT);
		}
		else
		{
			int *paramoutINT=(int*)MALLOC(sizeof(int));
			*paramoutINT=(int)FALSE;
			n1=1;
			CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
			LhsVar(1)=Rhs+1;
			FREE(paramoutINT);
		}

	}
	else
	{
		Scierror(999,MSG_ERROR14);
		return 0;
	}

	if (Rhs == 1)
	{
		if (param2) FREE(param2);
		param2=NULL;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEpoke _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *param3=NULL;

	int TypeVar1=GetType(1);
	int TypeVar2=GetType(2);

	Rhs=Max(0,Rhs);
	CheckRhs(2,3);
	CheckLhs(0,1);

	if (Rhs == 3)
	{
		int TypeVar3=GetType(3);
		if (TypeVar3 == sci_matrix)
		{
			GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
			if ( (m1 == 1) && (n1 == 1) )
			{
				param3=istk(l1);
			}
			else
			{
				Scierror(999,MSG_ERROR14);
				return 0;
			}
		}
		else
		{
			Scierror(999,MSG_ERROR14);
			return 0;
		}
	}

	if (Rhs == 2)
	{
		param3=(int*)MALLOC(sizeof(int));
		*param3=-1;
	}

	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings))
	{
		char *param1=NULL,*param2=NULL;
		
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
		param2=cstk(l1);

		if ( DDEPoke(param1,param2,*param3) )
		{
			int *paramoutINT=(int*)MALLOC(sizeof(int));
			*paramoutINT=(int)TRUE;
			n1=1;
			CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
			LhsVar(1)=Rhs+1;
			FREE(paramoutINT);
		}
		else
		{
			int *paramoutINT=(int*)MALLOC(sizeof(int));
			*paramoutINT=(int)FALSE;
			n1=1;
			CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
			LhsVar(1)=Rhs+1;
			FREE(paramoutINT);
		}
	}
	else
	{
		Scierror(999,MSG_ERROR14);
		return 0;
	}

	if (Rhs == 2)
	{
		if (param3) FREE(param3);
		param3=NULL;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEreq _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *param2=NULL;

	int TypeVar1=GetType(1);

	Rhs=Max(0,Rhs);
	CheckRhs(1,2);
	CheckLhs(0,1);

	if (Rhs == 2)
	{
		int TypeVar2=GetType(2);
		if (TypeVar2 == sci_matrix)
		{
			GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
			if ( (m1 == 1) && (n1 == 1) )
			{
				param2=istk(l1);
			}
			else
			{
				Scierror(999,MSG_ERROR14);
				return 0;
			}
		}
		else
		{
			Scierror(999,MSG_ERROR14);
			return 0;
		}
	}

	if (Rhs == 1)
	{
		param2=(int*)MALLOC(sizeof(int));
		*param2=-1;
	}

	if (TypeVar1 == sci_strings)
	{
		char Buffer[512];
		char *param1=NULL;
			
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		if ( DDERequest(param1,*param2,Buffer) )
		{
			char *output=NULL ;
			output=(char*)MALLOC((strlen(Buffer)+1)*sizeof(char));
			wsprintf(output,"%s",Buffer);
			CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
			FREE(output);
			LhsVar(1)=Rhs+1;
		}
		else
		{
			m1=0;
			n1=0;
			l1=0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
			LhsVar(1)=Rhs+1;
		}
	}
	else
	{
		Scierror(999,MSG_ERROR14);
		return 0;
	}

	if (Rhs == 1)
	{
		if (param2) FREE(param2);
		param2=NULL;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEIsOpen _PARAMS((char *fname))
{
	static int l1,n1,m1;
	
	Rhs=Max(0,Rhs);
	CheckRhs(0,0);
	CheckLhs(0,1);

	m1=n1=1;
	
	CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&l1);
	
	if ( DDEIsOpen() )
	{
		*istk(l1)=TRUE;
	}
	else
	{
		*istk(l1)=FALSE;
	}

	

	LhsVar(1)= Rhs+1;
	return 0;
}
/*-----------------------------------------------------------------------------------*/
