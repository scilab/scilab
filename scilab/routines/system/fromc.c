/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "fromc.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/ 
#define TRUE  1
#define FALSE 0
/*-----------------------------------------------------------------------------------*/ 
static int bFromC=FALSE;
/*-----------------------------------------------------------------------------------*/ 
int C2F(intfromc) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,1);

	if ( bFromC )
	{
		*paramoutINT=(int)(TRUE);
	}
	else
	{
		*paramoutINT=(int)(FALSE);
	}

	n1=1;
	CreateVarFromPtr(1, "b", &n1, &n1, &paramoutINT);
	LhsVar(1)=1;
	C2F(putlhsvar)();
	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void SetFromCToON(void)
{
	bFromC=(int)(TRUE);
}
/*-----------------------------------------------------------------------------------*/ 
int IsFromC(void)
{
	return bFromC;
}
/*-----------------------------------------------------------------------------------*/ 

