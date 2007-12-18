/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TextToPrint.h"
#include "gw_windows_tools.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "Scierror.h"
#include "../../../fileio/includes/FileExist.h"
#include "IsAScalar.h"
#include "freeArrayOfString.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
extern BOOL FigureToPrint(int figurenumber,BOOL Postscript);
/*--------------------------------------------------------------------------*/
int C2F(sci_toprint) _PARAMS((char *fname,unsigned long l))
{
	int l1 = 0, m1 = 0, n1 = 0;

	int *paramoutINT=(int*)MALLOC(sizeof(int));
	*paramoutINT=(int)FALSE;

	CheckRhs(1,2);
	CheckLhs(0,1);


	if (Rhs == 1)
	{
		if (GetType(1) == sci_strings)
		{
			char *param=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

			if ( n1==1 )
			{
				param=cstk(l1);
				if (FileExist(param))
				{
					PrintFile(param);
					*paramoutINT=(int)(TRUE);
				}
				else 
				{
					sciprint(_("\nError : Can't open %s.\n"),param);
					*paramoutINT=(int)(FALSE);
				}
			}
			else
			{
				Scierror(999,_("%: Wrong type for first input argument: a string expected.\n"),fname);
				return 0;
			}
		}
		else
		{
			if (GetType(1) == sci_matrix)
			{
				if (IsAScalar(1))
				{
					int num_win=-2;
					GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
					num_win=*istk(l1);
					if (num_win>=0)
					{
						/* DISABLE */
						sciprint(_("Not yet implemented.\n"));
						//*paramoutINT=(int)FigureToPrint(num_win,FALSE);
					}
					else
					{
						Scierror(999,_("%s: Wrong first input argument: >= 0 expected.\n"),fname);
						return 0;
					}
				}
				else
				{
					Scierror(999,_("%s: Wrong first input argument: see help toprint."),fname);
					return 0;
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong first input argument: see help toprint."),fname);
				return 0;
			}
		}
	}
	else
	{
		if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
		{
			char **Str=NULL;
			char *param2=NULL;
			char *lines=NULL;

			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

			if (n1 == 1)
			{
				int i=0;

				lines=MALLOC(m1*2048*sizeof(char));
				for (i = 0; i<m1 ;i++)
				{
					if (i==0)
					{
						sprintf(lines,"%s\n",Str[i]);
					}
					else 
					{
						sprintf(lines,"%s%s\n",lines,Str[i]);
					}
				}
				freeArrayOfString(Str,m1);
			}
			else
			if (m1 == 1)
			{
				int i=0;

				lines=MALLOC(n1*2048*sizeof(char));

				for (i = 0; i<n1 ;i++)
				{
					if (i==0)
					{
						sprintf(lines,"%s\n",Str[i]);
					}
					else sprintf(lines,"%s%s\n",lines,Str[i]);
				}
				freeArrayOfString(Str,m1);
			}
			else
			{
				freeArrayOfString(Str,m1);
				Scierror(999,"parameter incorrect must be a string or a string matrix (1 x m) or (n x 1).");
				return 0;
			}

			GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
			param2=cstk(l1);

			PrintString(lines,param2);
			if (lines){FREE(lines);lines=NULL;}
			*paramoutINT=(int)(TRUE);
		}
		else
		{
			if ( (GetType(1) == sci_matrix) && (GetType(2) == sci_strings) )
			{
				if (IsAScalar(1))
				{
					int num_win=-2;
					char *param=NULL;

					GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
					num_win=*istk(l1);

					GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
					param=cstk(l1);

					if ( (strcmp(param,"pos")==0) || (strcmp(param,"gdi")==0) )
					{
						if ( strcmp(param,"pos")==0 )
						{
							/* DISABLE */
							sciprint("Not yet implemented.\n");
							//*paramoutINT=(int)FigureToPrint(num_win,TRUE);
						}
						else
						{
							/* DISABLE */
							sciprint("Not yet implemented.\n");
							//*paramoutINT=(int)FigureToPrint(num_win,FALSE);
						}
					}
					else
					{
						Scierror(999,"2nd parameter incorrect --> see help toprint ('pos' or 'gdi').");
						return 0;
					}
				}
				else
				{
					Scierror(999,"1st parameter incorrect --> see help toprint.");
					return 0;
				}

			}
			else
			{
				Scierror(999,"parameter(s) incorrect --> see help toprint.");
				return 0;
			}
		}
	}

	n1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1,&paramoutINT);
	LhsVar(1)=Rhs+1;
	FREE(paramoutINT);
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
