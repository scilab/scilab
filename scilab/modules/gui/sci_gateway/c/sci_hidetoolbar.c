/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_gui.h"
#include "stack-c.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_hidetoolbar) _PARAMS((char *fname,unsigned long l))
{
	static int l1, m1, n1;	

	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( GetType(1) == sci_matrix )
	{
		int num_win=-2;

		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		num_win=*istk(l1);

		/* TO DO interface with java */

		LhsVar(1)=0;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,"Parameter incorrect type.\n");
		return 0;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
