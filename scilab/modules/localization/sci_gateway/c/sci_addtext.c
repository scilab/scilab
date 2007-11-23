/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "gw_localization.h"
#include "Scierror.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static struct hashtable *GetLocalizationHashTable(char *param1);
/*--------------------------------------------------------------------------*/
int C2F(sci_addtext) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	static int n2,m2;
	static int n3,m3;

	CheckRhs(3,3);
	CheckLhs(0,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) && (GetType(3) == sci_strings) )
	{
		char *param1=NULL;
		char **param2=NULL;
		char **param3=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		if ((strcmp(param1,"errors")==0) || (strcmp(param1,"messages")==0) || (strcmp(param1,"menus")==0))
		{
			struct hashtable *Table=NULL;

			Table=GetLocalizationHashTable(param1);

			if (Table)
			{
				GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&param2);
				GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&param3);
				if ( (m2==m3) && (n2==n3) )
				{
					int i=0;

					for (i=0;i<m2*n2;i++)
					{
						if (!AppendHashTableLocalization(Table,param2[i],param3[i],"MEMORY"))
						{
							n1=1;
							CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
							*istk(l1)=(int)(FALSE);
							LhsVar(1)=Rhs+1;
							C2F(putlhsvar)();
							return 0;

						}
					}

					/* it is okay ;) */
					n1=1;
					CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
					*istk(l1)=(int)(TRUE);
					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();
				}
				else
				{
					Scierror(999,"parameter 2 and 3 must have the same dimensions. See help addtext.\n");
					return 0;
				}
			}
			else
			{
				n1=1;
				CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
				*istk(l1)=(int)(FALSE);
				LhsVar(1)=Rhs+1;
				C2F(putlhsvar)();
			}
		}
		else
		{
			Scierror(999,"invalid first parameter. See help addtext.\n");
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
static struct hashtable *GetLocalizationHashTable(char *param1)
{
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
	return Table;
}
/*--------------------------------------------------------------------------*/
