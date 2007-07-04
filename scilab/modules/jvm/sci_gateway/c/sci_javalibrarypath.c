/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include "gw_jvm.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "addToLibrarypath.h"
#include "getLibrarypath.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_javalibrarypath) _PARAMS((char *fname,unsigned long fname_len))
{
	Rhs = Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		int nbRow=0;
		int nbCol=1;
		int i=0;
		char **Strings=NULL;

		Strings=getLibrarypath(&nbRow);
		CreateVarFromPtr( Rhs+1, "S", &nbRow, &nbCol,Strings ) ;

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		if (Strings)
		{
			for (i=0;i<nbRow;i++)
			{
				if (Strings[i])
				{
					FREE(Strings[i]);
					Strings[i]=NULL;
				}
			}
			FREE(Strings);
			Strings=NULL;
		}

	}
	else
	{
		if ( GetType(1) == sci_strings )
		{
			static int n1=0,m1=0;
			int i=0;
			BOOL bOK=FALSE;
			char **LIBRARYPATHS=NULL;

			GetRhsVar(1,"S",&m1,&n1,&LIBRARYPATHS);

			for (i = 0; i<m1*n1 ;i++)
			{
				bOK=addToLibrarypath(LIBRARYPATHS[i]);
				if (!bOK)
				{
					Scierror(999,"could not add path to java.library.path : %s.\r\n",LIBRARYPATHS[i]);
					return 0;
				}
			}
			LhsVar(1) = 0;
			C2F(putlhsvar)();	
		}
		else
		{
			Scierror(999,"invalid parameter(s).\r\n");
		}
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
