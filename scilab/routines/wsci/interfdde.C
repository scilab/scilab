/* Allan CORNET */
/* INRIA 2004 */
#include "interfdde.h"
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEopen _PARAMS((char *fname))
{
	static int l1,n1,m1;
	Rhs=Max(0,Rhs);
	CheckRhs(0,2);
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
				LhsVar(1)=0;
			}
			else
			{
				Scierror(999,"Error DDE.\r\n");
			}
			
		}
		else
		{
			Scierror(999,"parameters type incorrect.\r\n");
		}
	}
	else
	{
		Scierror(999,"2 parameters required.\r\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEclose _PARAMS((char *fname))
{
	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		if ( DDECloseConnection() )
		{
			LhsVar(1)=0;
		}
		else
		{
			Scierror(999,"DDE not use.\r\n");
		}
	}
	else
	{
		Scierror(999,"no parameter required.\r\n");
	}

	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEexec _PARAMS((char *fname))
{
	static int l1,n1,m1;
	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 1)
	{
		int TypeVar1=GetType(1);
	
		if (TypeVar1 == sci_strings)
		{
			char *param1=NULL;
			
			GetRhsVar(1,"c",&m1,&n1,&l1);
		    param1=cstk(l1);

			if ( DDEExecute(param1) )
			{
				LhsVar(1)=0;
			}
			else
			{
				Scierror(999,"Error DDE.\r\n");
			}
			
		}
		else
		{
			Scierror(999,"parameter type incorrect.\r\n");
		}
	}
	else
	{
		Scierror(999,"1 parameter required.\r\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEpoke _PARAMS((char *fname))
{
	static int l1,n1,m1;
	Rhs=Max(0,Rhs);
	CheckRhs(0,2);
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

			DDEPoke(param1,param2);
			LhsVar(1)=0;
			
		}
		else
		{
			Scierror(999,"parameters type incorrect.\r\n");
		}
	}
	else
	{
		Scierror(999,"2 parameters required.\r\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int	InterfaceWindowsDDEreq _PARAMS((char *fname))
{
	static int l1,n1,m1;
	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 1)
	{
		int TypeVar1=GetType(1);

		if (TypeVar1 == sci_strings)
		{
			char *param1=NULL;
			char Buffer[512];
			
			GetRhsVar(1,"c",&m1,&n1,&l1);
		    param1=cstk(l1);

			if ( DDERequest(param1,Buffer) )
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
				Scierror(999,"Error DDE.\r\n");
			}
			
		}
		else
		{
			Scierror(999,"parameter type incorrect.\r\n");
		}
	}
	else
	{
		Scierror(999,"1 parameter required.\r\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/