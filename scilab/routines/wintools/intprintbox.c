/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intprintbox.h"
#ifdef _MSC_VER
	#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
	#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
	extern BOOL ConfigurePrinterDialogBox(void);
#endif
/*-----------------------------------------------------------------------------------*/
/* Open Print DialogBox*/
int C2F(intprintsetupbox) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	CheckRhs(0,0);
	CheckLhs(0,1);

	#if _MSC_VER
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
	CreateVarFromPtr(1, "b", &n1, &n1,&paramoutINT);
	LhsVar(1)=1;
	FREE(paramoutINT);
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
