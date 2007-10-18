/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "getlongpathname.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getlongpathname) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;

	CheckRhs(0,1);
	CheckLhs(1,2);

	if (GetType(1) == sci_strings)
	{
		int bOK=FALSE;
		char *LongName=NULL;
		char *ShortName=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		ShortName=cstk(l1);

		LongName = getlongpathname(ShortName,&bOK);

		n1=1;
		CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(LongName), &m1),&n1,&LongName);
		LhsVar(1)=Rhs+1;

		if (LongName) {FREE(LongName);LongName=NULL;}

		if (Lhs != 1) /* Lhs == 2 */
		{
			int *bOkOutINT=(int*)MALLOC(sizeof(int));

			*bOkOutINT=bOK;
			n1=1;
			CreateVarFromPtr(Rhs+2,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &bOkOutINT);
			LhsVar(2)=Rhs+2;
			if (bOkOutINT) {FREE(bOkOutINT);bOkOutINT=NULL;}
		}
		C2F(putlhsvar)();
	}
	else 
	{
		Scierror(999,_("Not a path string"));
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
