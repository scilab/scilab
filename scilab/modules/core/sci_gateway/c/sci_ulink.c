/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "error_scilab.h"
#include "link.h"
/*-----------------------------------------------------------------------------------*/
static int intulink (char *fname,unsigned long fname_len);
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_ulink) _PARAMS((char *fname,unsigned long fname_len))
{
	intulink(fname,fname_len);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int intulink (char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;

	Rhs = Max(0, Rhs);
	CheckRhs(0,1);
	CheckLhs(1,1);

	if (Rhs == 0)
	{
		ulinkall();
	}
	else
	{
		if ( GetType(1) == sci_matrix )
		{
			double *params=NULL;
			int i=0;
			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
			params=stk(l1);
			for(i=0;i<m1*n1;i++)
			{
				integer ilib=(integer)params[i];
				C2F(isciulink)(&ilib);
			}
		}
		else
		{
			error_scilab(999,_("Parameter must be a integer."));
			return 0;
		}
	}

	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
