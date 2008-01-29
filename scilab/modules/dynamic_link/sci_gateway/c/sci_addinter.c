/*-----------------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_dynamic_link.h"
#include "machine.h"
#include "stack-c.h"
#include "addinter.h"
#include "localization.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_addinter) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(3,3);
	CheckLhs(1,1);

	if ( (VarType(1) == sci_strings) && 
		(VarType(2) == sci_strings) && 
		(VarType(3) == sci_strings) )
	{
		char **sharedlibname = NULL;
		char **spname = NULL;
		char **fcts = NULL;

		int m1 = 0, n1 = 0, l1 = 0;
		int m2 = 0, n2 = 0, l2 = 0;
		int m3 = 0, n3 = 0, l3 = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&sharedlibname);
		if ( (m1 != n1) && (n1 != 1) )
		{
			Scierror(999,_("%s: Wrong size for first input argument.\n"),fname);
			return 0;
		}

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&spname);
		GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&fcts);

		if ( (m2 != n2) && (n2 != 1) )
		{
			Scierror(999,_("%s: Wrong size for second input argument.\n"),fname);
			return 0;
		}

		{
			int ierr = 0;

			ierr = AddInterfaceToScilab(sharedlibname[0],spname[0],fcts,n3);
			if (ierr == 0)
			{
				LhsVar(1) = 0;
				C2F(putlhsvar)();
			}
			else
			{
				switch (ierr)
				{
				default :
					Scierror(999,_("%s : unknow error.\n"),fname);
					break;
				}
			}
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input arguments: Strings expected.\n"),fname); 
		return 0;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
