/* Allan CORNET */
/* INRIA 2004 */
#include "interfdde.h"

#define MsgErrortype1 "Error DDE.\r\n"
#define MsgErrortype2 "parameter(s) type incorrect.\r\n"
#define MsgErrortype3 "2 parameters required.\r\n"
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
			
			GetRhsVar(1,"c",&m1,&n1,&l1);
		    param1=cstk(l1);

			GetRhsVar(2,"c",&m1,&n1,&l1);
		    param2=cstk(l1);

			if ( DDEOpenConnection(param1,param2) )
			{
				int *paramoutINT=(int*)malloc(sizeof(int));
				*paramoutINT=(int)(TRUE);
				n1=1;
				CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
				LhsVar(1)=1;
				free(paramoutINT);
			}
			else
			{
				int *paramoutINT=(int*)malloc(sizeof(int));
				*paramoutINT=(int)(FALSE);
				n1=1;
				CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
				LhsVar(1)=1;
				free(paramoutINT);
			}
		}
		else 
		{
			Scierror(999,MsgErrortype2);
		}
	}
	else
	{
		Scierror(999,MsgErrortype3);
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
		int *paramoutINT=(int*)malloc(sizeof(int));
		*paramoutINT=(int)TRUE;
		n1=1;
		CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
		LhsVar(1)=1;
		free(paramoutINT);
	}
	else
	{
		int *paramoutINT=(int*)malloc(sizeof(int));
		*paramoutINT=(int)FALSE;
		n1=1;
		CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
		LhsVar(1)=1;
		free(paramoutINT);
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
			GetRhsVar(2,"i",&m1,&n1,&l1);
			if ( (m1 == 1) && (n1 == 1) )
			{
				param2=istk(l1);
			}
			else
			{
				Scierror(999,MsgErrortype2);
				return 0;
			}
		}
		else
		{
			Scierror(999,MsgErrortype2);
			return 0;
		}
	}

	if (Rhs == 1)
	{
		param2=malloc(sizeof(int));
		*param2=-1;
	}

	if (TypeVar1 == sci_strings)
	{
		char *param1=NULL;
			
		GetRhsVar(1,"c",&m1,&n1,&l1);
		param1=cstk(l1);

		if ( DDEExecute(param1,*param2) )
		{
			int *paramoutINT=(int*)malloc(sizeof(int));
			*paramoutINT=(int)TRUE;
			n1=1;
			CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
			LhsVar(1)=1;
			free(paramoutINT);
		}
		else
		{
			int *paramoutINT=(int*)malloc(sizeof(int));
			*paramoutINT=(int)FALSE;
			n1=1;
			CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
			LhsVar(1)=1;
			free(paramoutINT);
		}

	}
	else
	{
		Scierror(999,MsgErrortype2);
		return 0;
	}

	if (Rhs == 1)
	{
		if (param2) free(param2);
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
			GetRhsVar(3,"i",&m1,&n1,&l1);
			if ( (m1 == 1) && (n1 == 1) )
			{
				param3=istk(l1);
			}
			else
			{
				Scierror(999,MsgErrortype2);
				return 0;
			}
		}
		else
		{
			Scierror(999,MsgErrortype2);
			return 0;
		}
	}

	if (Rhs == 2)
	{
		param3=(int*)malloc(sizeof(int));
		*param3=-1;
	}

	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings))
	{
		char *param1=NULL,*param2=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		param1=cstk(l1);

		GetRhsVar(2,"c",&m1,&n1,&l1);
		param2=cstk(l1);

		if ( DDEPoke(param1,param2,*param3) )
		{
			int *paramoutINT=(int*)malloc(sizeof(int));
			*paramoutINT=(int)TRUE;
			n1=1;
			CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
			LhsVar(1)=1;
			free(paramoutINT);
		}
		else
		{
			int *paramoutINT=(int*)malloc(sizeof(int));
			*paramoutINT=(int)FALSE;
			n1=1;
			CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
			LhsVar(1)=1;
			free(paramoutINT);
		}
	}
	else
	{
		Scierror(999,MsgErrortype2);
		return 0;
	}

	if (Rhs == 2)
	{
		if (param3) free(param3);
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
			GetRhsVar(2,"i",&m1,&n1,&l1);
			if ( (m1 == 1) && (n1 == 1) )
			{
				param2=istk(l1);
			}
			else
			{
				Scierror(999,MsgErrortype2);
				return 0;
			}
		}
		else
		{
			Scierror(999,MsgErrortype2);
			return 0;
		}
	}

	if (Rhs == 1)
	{
		param2=(int*)malloc(sizeof(int));
		*param2=-1;
	}

	if (TypeVar1 == sci_strings)
	{
		char Buffer[512];
		char *param1=NULL;
			
		GetRhsVar(1,"c",&m1,&n1,&l1);
		param1=cstk(l1);

		if ( DDERequest(param1,*param2,Buffer) )
		{
			char *output=NULL ;
			output=(char*)malloc((strlen(Buffer)+1)*sizeof(char));
			wsprintf(output,"%s",Buffer);
			CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);
			free(output);
			LhsVar(1)=1;
		}
		else
		{
			m1=0;
			n1=0;
			l1=0;
			CreateVar(1,"d",  &m1, &n1, &l1);
			LhsVar(1)=1;
		}
	}
	else
	{
		Scierror(999,MsgErrortype2);
		return 0;
	}

	if (Rhs == 1)
	{
		if (param2) free(param2);
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
	
	CreateVar(1,"b",&m1,&n1,&l1);
	
	if ( DDEIsOpen() )
	{
		*istk(l1)=TRUE;
	}
	else
	{
		*istk(l1)=FALSE;
	}

	

	LhsVar(1)= 1;
	return 0;
}
/*-----------------------------------------------------------------------------------*/