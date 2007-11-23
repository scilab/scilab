/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h" 
#include "removedir.h"
#include "isdir.h"
#include "Scierror.h"
#include "cluni0.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_removedir) _PARAMS((char *fname,unsigned long l))
{	
	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)
	{
		BOOL bOK = FALSE;
		int m1 = 0, n1 = 0, l1 = 0;
		char expandedpath[PATH_MAX+1];
		int out_n = 0;
		char *VarName = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);

		C2F(cluni0)(VarName,expandedpath, &out_n,(int)strlen(VarName),PATH_MAX);

		if ( isdir(expandedpath) )
		{
			bOK = removedir(expandedpath);
		}

		m1 = 1; n1 = 1;
		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1 ,&l1);
		*istk(l1) = bOK;

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();		
	}
	else
	{
		Scierror(999,_("Invalid parameter , it must be a directory (string).\n"));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
