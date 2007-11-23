/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_pathsep) _PARAMS((char *fname,unsigned long fname_len))
{
	static int n1,m1;
	char *separator=NULL;

	CheckRhs(0,0);
	CheckLhs(1,1);

	separator=(char*)MALLOC(sizeof(char)*(strlen(PATH_SEPARATOR)+1));
	if (separator) strcpy(separator,PATH_SEPARATOR);
	
	n1 = 1;
	CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(separator), &m1),&n1,&separator);
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();

	if (separator) {FREE(separator);separator=NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
