/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intprintbox.h"
#if defined (linux)
#include <string.h>
#endif

/*-----------------------------------------------------------------------------------*/
#if WIN32
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

	#if WIN32
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
