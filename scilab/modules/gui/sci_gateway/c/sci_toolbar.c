/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_gui.h"
#include "stack-c.h"
#include "Scierror.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_toolbar) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	char *Output=NULL;

	CheckRhs(1,2);
	CheckLhs(0,1);

	Output=(char*)MALLOC(4*sizeof(char));

	if (Rhs==1)
	{
		if ( GetType(1) == sci_matrix )
		{
			int numwin=-2;
			GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);

			numwin=*istk(l1);

			/* TO DO interface with java */

			strcpy(Output,"off");
		}
		else
		{
			Scierror(999,"parameter type incorrect");
			return 0;
		}
	}
	else /*Rhs == 2 */
	{
		if ( (GetType(1) == sci_matrix) && (GetType(2) == sci_strings) )
		{
			int numwin=-2;
			char *param=NULL;

			GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
			numwin=*istk(l1);

			GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
			param=cstk(l1);

			if ( (strcmp(param,"off")==0) || (strcmp(param,"on")==0) )
			{
				/* TO DO interface with java */

				strcpy(Output,"off");
			}
			else
			{
				Scierror(999,"Second parameter incorrect: 'on' or 'off'");
				return 0;
			}
		}
		else
		{
			Scierror(999,"parameter(s) type incorrect");
			return 0;
		}
	}

	n1=1;
	CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
