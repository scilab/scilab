/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_setlanguage.h"
#include "setgetlanguage.h"
#include "Scierror.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_setlanguage) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;

	CheckRhs(0,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *param=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		param=cstk(l1);
		if (!setlanguage(param))
		{
			sciprint("%s is a invalid language. see 'help setlanguage'.\n\n");
			CreateVar(Rhs+1, "b", &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}
		else
		{
			CreateVar(Rhs+1, "b", &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,"parameter incorrect: see 'help setlanguage'");
		return 0;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
