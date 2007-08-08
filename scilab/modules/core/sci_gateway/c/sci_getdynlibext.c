/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "gw_core.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_getdynlibext) _PARAMS((char *fname,unsigned long fname_len))
{
	#define nbcharsext 8 
	static int n1,m1;
	char *output=NULL ;

	CheckRhs(0,0);
	CheckLhs(1,1);

	output=(char*)MALLOC((nbcharsext+1)*sizeof(char));
	sprintf(output,"%s",SHARED_LIB_EXT);
	
	n1=1;
	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
	if (output) {FREE(output);output=NULL;}

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
