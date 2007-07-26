/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "librarieslist.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_librarieslist) _PARAMS((char *fname,unsigned long fname_len))
{
	
	char **libraries = NULL;
	int sizelibraries = 0;

	CheckRhs(0,0);
	CheckLhs(0,1);

	libraries = getlibrarieslist(&sizelibraries);

	if (libraries)
	{
		int m = 0, n = 0;
		m = sizelibraries; n = 1;
		CreateVarFromPtr(Rhs+1, "S", &m, &n, libraries);
	}
	else
	{
		int m = 0, n = 0, l = 0;
		CreateVarFromPtr(Rhs+1, "d",&n,&m,&l);
	}
	LhsVar(1) = Rhs+1;

	C2F(putlhsvar)();

	if (libraries)
	{
		int i = 0;

		for (i=0;i < sizelibraries;i++) 
		{ 
			if (libraries[i])
			{
				FREE(libraries[i]);
				libraries[i]=NULL; 
			}
		}
		FREE(libraries);
		libraries = NULL; 
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
