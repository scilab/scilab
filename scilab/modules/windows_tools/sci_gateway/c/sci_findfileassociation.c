/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_windows_tools.h"
#include "MALLOC.h" 
#include "Scierror.h"
#include "FindFileAssociation.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_findfileassociation) _PARAMS((char *fname,unsigned long l))
{	
	static int l1,n1,m1;
	char *Output=NULL;
	char ExtraParam[PATH_MAX];
	char AssocParam[PATH_MAX];

	Rhs=Max(Rhs,0);
	CheckRhs(1,2);
	CheckLhs(0,1);

	if (Rhs == 1)
	{
		if (GetType(1) != sci_strings)
		{
			Scierror(999,"invalid parameter.\n");
			return 0;
		}
		else
		{
			char *param=NULL;

			GetRhsVar(1,"c",&m1,&n1,&l1);
			param=cstk(l1);
			strcpy(AssocParam,param);

			strcpy(ExtraParam,"open");
		}
	}
	else
	{
		if ( (GetType(1) != sci_strings) && (GetType(2) != sci_strings) )
		{
			Scierror(999,"invalid parameter(s).\n");
			return 0;
		}
		else
		{
			char *param=NULL;

			GetRhsVar(1,"c",&m1,&n1,&l1);
			param=cstk(l1);

			strcpy(AssocParam,param);

			GetRhsVar(2,"c",&m1,&n1,&l1);
			param=cstk(l1);

			strcpy(ExtraParam,param);
		}
	}

	Output=FindFileAssociation(AssocParam,ExtraParam) ;

	if (Output)
	{
		n1=1;
		CreateVarFromPtr(Rhs+ 1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	}
	else
	{
		n1=0;
		m1=0;
		l1=0;
		CreateVarFromPtr(Rhs+ 1, "d",&n1,&m1,&l1);
	}
		
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
