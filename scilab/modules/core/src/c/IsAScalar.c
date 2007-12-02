/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "stack-c.h"
#include "IsAScalar.h"
/*--------------------------------------------------------------------------*/ 
int IsAScalar(int RhsNumber)
{

	if ( GetType(RhsNumber) == sci_matrix )
	{
		static int l1,n1,m1;
		GetRhsVar(RhsNumber,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		if ( (m1 == 1) && (n1 == 1) )
		{
			return 1;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
