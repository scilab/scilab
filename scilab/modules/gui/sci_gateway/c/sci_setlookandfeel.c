/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "gw_gui.h"
#include "setlookandfeel.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_setlookandfeel) _PARAMS((char *fname,unsigned long fname_len))
{
	Rhs = Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);


	if (Rhs == 0)
	{
		BOOL bOK = FALSE;
		int n1 = 1;
		int *paramoutINT=(int*)MALLOC(sizeof(int));

		bOK = setsystemlookandfeel();
		*paramoutINT=(int)bOK;

		n1=1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
		LhsVar(1)=Rhs+1;

		C2F(putlhsvar)();

		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}

	}
	else if ( GetType(1) == sci_strings )
	{
		int m1,n1=0,l1=0;
		BOOL bOK = FALSE;
		char *looknfeel = NULL;
		int *paramoutINT=(int*)MALLOC(sizeof(int));

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		looknfeel=cstk(l1);

		bOK = setlookandfeel(looknfeel);
		*paramoutINT=(int)bOK;

		n1=1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
		LhsVar(1)=Rhs+1;

		C2F(putlhsvar)();

		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	}
	else
	{
		Scierror(999,"invalid parameter.\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
