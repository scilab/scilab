/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "sci_setlanguage.h"
#include "setgetlanguage.h"
#include "Scierror.h"
#include "sciprint.h"
#include "warningmode.h"
#include "defaultlanguage.h"
#include "MALLOC.h"
#include "loadhashtableslocalization.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_setlanguage) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;

	CheckRhs(0,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *param=NULL;
		char *newlanguage=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param=cstk(l1);

		newlanguage=convertlanguagealias(param);

		if ( LanguageIsOK(param) && (newlanguage == NULL) )
		{
		  newlanguage=param;
		}

		if (newlanguage)
		{
			if (needtochangelanguage(newlanguage))
			{
				if (!setlanguage(newlanguage,TRUE,TRUE))
				{
					CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
					*istk(l1)=(int)(FALSE);
				}
				else
				{
					CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
					*istk(l1)=(int)(TRUE);
				}
			}
			else
			{
				/* do nothing */
				CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
				*istk(l1)=(int)(TRUE);
			}
		}
		else
		{
			if (getWarningMode())
			{
				sciprint("\nUnsupported language \"%s\".\n",param);
				sciprint("Choosing default language : \"%s\".\n\n",SCILABDEFAULTLANGUAGE);
			}

			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
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
