/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TextToPrint.h"
#include "gw_windows_tools.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "Scierror.h"
#include "../../../fileio/includes/FileExist.h"
/*-----------------------------------------------------------------------------------*/
extern int IsAScalar(int RhsNumber);
extern BOOL FigureToPrint(int figurenumber,BOOL Postscript);
/*-----------------------------------------------------------------------------------*/
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
					sciprint("\nError : Can't open %s.\n",param);
					*paramoutINT=(int)(FALSE);
				}
			}
			else
			{
				Scierror(999,"parameter incorrect must be a string (a filename).");
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
						sciprint("Not yet implemented.\n");
						//*paramoutINT=(int)FigureToPrint(num_win,FALSE);
					}
					else
					{
						Scierror(999,"parameter must be >= 0.");
						return 0;
					}
				}
				else
				{
					Scierror(999,"parameter incorrect --> see help toprint.");
					return 0;
				}
			}
			else
			{
				Scierror(999,"parameter incorrect --> see help toprint.");
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
				if (Str)
				{
					for (i = 0; i<m1 ;i++)
					{
						if (Str[i])
						{
							FREE(Str[i]);
							Str[i]=NULL;
						}
					}
					FREE(Str);
					Str=NULL;
				}
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
				if (Str)
				{
					for (i = 0; i<n1 ;i++)
					{
						if (Str[i])
						{
							FREE(Str[i]);
							Str[i]=NULL;
						}
					}
					FREE(Str);
					Str=NULL;
				}
			}
			else
			{
				if (Str)
				{
					int i=0;
					for (i = 0; i<n1 ;i++)
					{
						if (Str[i])
						{
							FREE(Str[i]);
							Str[i]=NULL;
						}
					}
					FREE(Str);
					Str=NULL;
				}
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
/*-----------------------------------------------------------------------------------*/
