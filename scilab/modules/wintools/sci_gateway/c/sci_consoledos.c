/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_wintools.h"
#include "MALLOC.h" /* MALLOC */
#include "sciprint.h"
#ifdef _MSC_VER
#include "WinConsole.h"
#endif
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_consoledos) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	char *Output=NULL;

	Rhs=Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);

	Output=(char*)MALLOC(4*sizeof(char));

	if (Rhs==0)
	{
		#ifdef _MSC_VER
		if (IsWindowInterface()) 
			{
				if (GetConsoleState()) strcpy(Output,"on");
				else strcpy(Output,"off");
					
			}
			else
			{
				sciprint("Only on Windows Mode, not in Console Mode.\n");
				strcpy(Output,"off");
			}
		#else
			strcpy(Output,"off");
		#endif
	}
	else
	if (GetType(1) == sci_strings)
		{
			char *param=NULL;

			GetRhsVar(1,"c",&m1,&n1,&l1);
			param=cstk(l1);

			if ( (strcmp(param,"off")==0) || (strcmp(param,"on")==0) )
			{
				#ifdef _MSC_VER
				if (IsWindowInterface()) 
					{
						if (strcmp(param,"on")==0)
							{
								SetConsoleState(1);
								ShowScilex();
								strcpy(Output,"on");
							}
						else
							{
								SetConsoleState(0);
								HideScilex();
								strcpy(Output,"off");
							}
					}
				else
					{
						sciprint("Only on Windows Mode, not in Console Mode.\n");
						strcpy(Output,"off");
					}
				#else			
					strcpy(Output,"off");
				#endif
			}
			else
			{
				Scierror(999,"parameter incorrect: 'on' or 'off'");
				return 0;
			}
		}
		else
		{
			Scierror(999,"parameter incorrect: 'on' or 'off'");
			return 0;
		}

	n1=1;
	CreateVarFromPtr(Rhs+ 1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
