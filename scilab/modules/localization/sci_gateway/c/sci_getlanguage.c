/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "sci_getlanguage.h"
#include "setgetlanguage.h"
#include "defaultlanguage.h"
#include "Scierror.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getlanguage) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	char *Output=NULL;

	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 1)
	{
		if (GetType(1) == sci_strings)
		{
			char *param=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			param=cstk(l1);

			if (strcmp(param,"LANGUAGE_DEFAULT")==0)
			{
			  Output=SCILABDEFAULTLANGUAGE;
			}
			else
			{
				Scierror(999,"invalid parameter. see 'help getlanguage'.\n\n");
				return 0;
			}
		}
		else
		{
			Scierror(999,"invalid parameter. see 'help getlanguage'.\n\n");
			return 0;
		}
	}
	else
	{
		Output=getlanguage();
	}

	n1=1;
	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
