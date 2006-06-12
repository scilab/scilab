/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
#include "intwarning.h"
/*-----------------------------------------------------------------------------------*/ 

/*-----------------------------------------------------------------------------------*/ 
#define TRUE  1
#define FALSE 0
/*-----------------------------------------------------------------------------------*/ 
int C2F(intwarning) __PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( GetType(1) == sci_strings )
	{
		char *Param=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		Param=cstk(l1);
		sciprint("WARNING:%s\r\n",Param);

	}
	else
	{
		Scierror(999,"Parameter must be a string.\r\n");
		return 0;
	}
	
	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
