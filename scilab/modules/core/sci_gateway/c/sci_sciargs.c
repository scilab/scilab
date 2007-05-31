/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "getcommandlineargs.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_sciargs) _PARAMS((char *fname,unsigned long fname_len))
{
	char **arguments = NULL;
	int nb_arguments = 0;
	
	int ncol = 0;
	int nrow = 0;

	arguments = getCommandLineArgs(&nb_arguments);

	nrow = 1;
	ncol = nb_arguments;
	CreateVarFromPtr(Rhs+1, "S", &nrow, &ncol,arguments);
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();

	if (arguments)
	{
		int i = 0;
		for (i = 0;i < nb_arguments; i++)
		{
			if (arguments[i])
			{
				FREE(arguments[i]);
				arguments[i] = NULL;
			}
		}
		FREE(arguments);
		arguments = NULL;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
