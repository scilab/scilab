/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_getdynlibext) _PARAMS((char *fname,unsigned long fname_len))
{
	#define nbcharsext 8 
	static int n1,m1;
	char *output=NULL ;

	CheckRhs(0,0);
	CheckLhs(1,1);

	output=(char*)MALLOC((nbcharsext+1)*sizeof(char));
	#if _MSC_VER
		sprintf(output,"%s",".dll");
	#else
		sprintf(output,"%s",".so");
	#endif

	n1=1;
	CreateVarFromPtr( 1, "c",(m1=(int)strlen(output), &m1),&n1,&output);
	if (output) {FREE(output);output=NULL;}

	LhsVar(1) = 1;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
