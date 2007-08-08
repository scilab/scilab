/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "gw_localization.h"
#include "QueryStringError.h"
#include "QueryStringMenu.h"
#include "QueryStringMessage.h"
#include "hashtable.h"
#include "getentrieshashtable.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
static int OneRhs_gettext(void);
static int TwoRhs_gettext(void);
static BOOL FreeElmsArray(char **ElemsArray,int nbrElems);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_gettext) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(1,2);
	CheckLhs(1,2);

	if (Rhs == 1)
	{
		OneRhs_gettext();
	}
	else /* Rhs == 2 */
	{
		TwoRhs_gettext();
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int TwoRhs_gettext(void)
{
	if (Lhs == 2)
	{
		error_scilab(78,"core_error_78","gettext");
		return 0;
	}

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		static int l1,n1,m1;

		char *param1=NULL;
		char *param2=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
		param2=cstk(l1);

		if ((strcmp(param1,"errors")==0) || (strcmp(param1,"messages")==0) || (strcmp(param1,"menus")==0))
		{
			char *TranslatedString=NULL;

			if (strcmp(param1,"errors")==0)
			{
				TranslatedString=QueryStringError(param2);		
			}
			else
			if (strcmp(param1,"messages")==0)
			{
				TranslatedString=QueryStringMessage(param2);	
			}
			else /* menus */
			{
				TranslatedString=QueryStringMenu(param2);	
			}

			if (TranslatedString)
			{
				n1=1;
				CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(TranslatedString), &m1),&n1,&TranslatedString);
				LhsVar(1)=Rhs+1;
				C2F(putlhsvar)();	
				if (TranslatedString) {FREE(TranslatedString);TranslatedString=NULL;}
			}
			else
			{
				m1=0;
				n1=0;
				l1=0;
				CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
				LhsVar(1)=Rhs+1;
				C2F(putlhsvar)();
			}
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
/*-----------------------------------------------------------------------------------*/
static int OneRhs_gettext(void)
{
	if (GetType(1) == sci_strings)
	{
		static int l1,n1,m1;
		char *param1=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		if ((strcmp(param1,"errors")==0) || (strcmp(param1,"messages")==0) || (strcmp(param1,"menus")==0))
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

			if (Table)
			{
				int numberselemTAGS=0;
				int numberselemSTRINGS=0;
				char **Tags=NULL;
				char **Strings=NULL;
				int n=0;
				int m=0;

				Tags=getTAGSinhashtable(Table,&numberselemTAGS);
				Strings=getSTRINGSinhashtable(Table,&numberselemSTRINGS);

				if (numberselemSTRINGS != numberselemTAGS)
				{
					Scierror(999,"Problem(s) with hashtable (1).\n");
					return 0;
				}
				else
				{
					
					m=numberselemTAGS;
					n=1;

					CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m, &n,Tags);
					LhsVar(1) = Rhs+1;

					if (Lhs == 2)
					{
						m=numberselemSTRINGS;
						n=1;

						CreateVarFromPtr(Rhs+2,MATRIX_OF_STRING_DATATYPE, &m, &n, Strings);
						LhsVar(2) = Rhs+2;
					}

					C2F(putlhsvar)();
				}

				FreeElmsArray(Tags,numberselemTAGS);
				FreeElmsArray(Strings,numberselemSTRINGS);

			}
			else
			{
				Scierror(999,"Problem(s) with hashtable (2).\n");
				return 0;
			}
		}
		else
		{
			Scierror(999,"invalid parameter. See help gettext.\n");
			return 0;
		}
	}
	else
	{
		Scierror(999,"invalid first parameter. See help gettext.\n");
		return 0;
	}
		
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static BOOL FreeElmsArray(char **ElemsArray,int nbrElems)
{
	BOOL bOK=FALSE;

	if (ElemsArray)
	{
		int i=0;
		for (i=0;i<nbrElems;i++)
		{
			if (ElemsArray[i])
			{
				FREE(ElemsArray[i]);
				ElemsArray[i]=NULL;
			}
		}
		FREE(ElemsArray);
		ElemsArray=NULL;
		bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
