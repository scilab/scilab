/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_localization.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_removetext) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;

	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		char *param1=NULL;
		char *param2=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
		param2=cstk(l1);

		if ((strcmp(param1,"errors")==0) || (strcmp(param1,"messages")==0) || (strcmp(param1,"menus")==0))
		{
			BOOL bOK=FALSE;
			struct hashtable *Table=NULL;

			if (strcmp(param1,"errors")==0)
			{
				Table=GetHashTableScilabErrors();	
			}
			else
			if (strcmp(param1,"messages")==0)
			{
				Table=GetHashTableScilabMessages();	
			}
			else /* menus */
			{
				Table=GetHashTableScilabMenus();	
			}

			if (Table) bOK=RemoveHastable_string(Table,param2);

			n1=1;
			if (bOK)
			{
				CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
				*istk(l1)=(int)(TRUE);
			}
			else
			{
				CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
				*istk(l1)=(int)(FALSE);
			}

			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();

		}
		else
		{
			Scierror(999,"invalid first parameter. See help gettext.\n");
			return 0;
		}
	}
	else
	{
		Scierror(999,"invalid parameter(s).\n");
		return 0;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
