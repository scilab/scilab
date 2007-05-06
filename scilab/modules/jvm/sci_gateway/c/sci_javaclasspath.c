/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include "gw_jvm.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"
#include "Scierror.h"
#include "addToClasspath.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_javaclasspath) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)
	{
		static int l1=0,n1=0,m1=0;
		int i=0;
		BOOL bOK=FALSE;
		char **CLASSPATHS=NULL;

		GetRhsVar(1,"S",&m1,&n1,&CLASSPATHS);

		for (i = 0; i<m1*n1 ;i++)
		{
			bOK=addToClasspath(CLASSPATHS[i]);
			if (!bOK)
			{
				Scierror(999,"could not add URL to system classloader : %s.\r\n",CLASSPATHS[i]);
				return 0;
			}
		}
		LhsVar(0) = 0;
		C2F(putlhsvar)();	
	}
	else
	{
		Scierror(999,"invalid parameter(s).\r\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
