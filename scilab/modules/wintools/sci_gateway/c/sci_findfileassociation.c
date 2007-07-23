/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_wintools.h"
#include "MALLOC.h" 
#include "Scierror.h"

/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include "FindFileAssociation.h"
#endif
/*-----------------------------------------------------------------------------------*/
/**
* returns full name of application associated to a file extension
* example : FindFileAssociation (".sce","print");
* returns "c:\programs files\scilab-5.0\wscilex.exe"
*/
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

	#ifdef _MSC_VER
	Output=FindFileAssociation(AssocParam,ExtraParam) ;
	#else
	/* returns [] for linux and others */
	#endif


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
