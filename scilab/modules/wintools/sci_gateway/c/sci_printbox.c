/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
	extern BOOL ConfigurePrinterDialogBox(void);
#endif
/*-----------------------------------------------------------------------------------*/
/* Open Print DialogBox*/
int C2F(sci_printsetupbox) _PARAMS((char *fname,unsigned long l))
{
	static int n1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	CheckRhs(0,0);
	CheckLhs(0,1);

	#ifdef _MSC_VER
		if (ConfigurePrinterDialogBox())
		{
			*paramoutINT=(int)(TRUE);
		}
		else
		{
			*paramoutINT=(int)(FALSE);
		}
	#else
		*paramoutINT=(int)(FALSE);
	#endif

	n1=1;
	CreateVarFromPtr(Rhs+1, "b", &n1, &n1,&paramoutINT);
	LhsVar(1)=Rhs+1;
	FREE(paramoutINT);
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
