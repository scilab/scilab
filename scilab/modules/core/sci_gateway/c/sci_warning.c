/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"
#include "Scierror.h"
#include "warningmode.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_warning) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( GetType(1) == sci_strings )
	{
		char *Param=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		Param=cstk(l1);

		if ( (strcmp(Param,"off") == 0) || (strcmp(Param,"on") == 0) )
		{
			if (strcmp(Param,"off") == 0)
			{
				setWarningMode(FALSE);
			}
			else  /* on */
			{
				setWarningMode(TRUE);
			}
		}
		else
		{
			if (strcmp(Param,"query") == 0)
			{
				char *Output=NULL;

				Output=(char*)MALLOC(4*sizeof(char));

				if (getWarningMode())
				{
					strcpy(Output,"on");
				}
				else
				{
					strcpy(Output,"off");
				}

				n1=1;
				CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
				LhsVar(1) = Rhs+1;
				C2F(putlhsvar)();
				if (Output) {FREE(Output);Output=NULL;}
				return 0;
			}
			else if ( getWarningMode() )
			{
				sciprint(_("WARNING : %s"),Param);
				sciprint("\n");
			}
		}
	}
	else
	{
		Scierror(999,_("Parameter must be a string.\n"));
		return 0;
	}
	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
