/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TextToPrint.h"
#include "gw_windows_tools.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_printsetupbox) _PARAMS((char *fname,unsigned long l))
{
	static int n1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	CheckRhs(0,0);
	CheckLhs(0,1);

	if (ConfigurePrinterDialogBox())
	{
		*paramoutINT=(int)(TRUE);
	}
	else
	{
		*paramoutINT=(int)(FALSE);
	}

	n1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1,&paramoutINT);
	LhsVar(1)=Rhs+1;
	FREE(paramoutINT);
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
