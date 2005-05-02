/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intmcisendstring.h"
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
	#include <windows.h>
	#pragma comment(lib, "winmm.lib")
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(intmcisendstring) _PARAMS((char *fname))
{
	int m1,n1,l1;
	char *Output=NULL;

	CheckRhs(1,1);
  
	if (GetType(1) == sci_strings)	
	{
		#if WIN32
		MCIERROR Error;
		char *MCICommand=NULL;
		
		char ReturnString[2048];
		GetRhsVar(1,"c",&m1,&n1,&l1);
		MCICommand=cstk(l1);

		Error=mciSendString(MCICommand,(LPSTR)ReturnString,sizeof(ReturnString),NULL);
		if (Error)
		{
			char ErrorText[128];
			if ( mciGetErrorString(Error,ErrorText,sizeof(ErrorText)) == FALSE )
			{
				wsprintf(ErrorText,"%s","Unkown MCI error");
			}
			Output=(char*)malloc((strlen(ErrorText)+1)*sizeof(char));
			wsprintf(Output,"%s",ErrorText);
		}
		else
		{
			if (strcmp(ReturnString,"")==0)
			{
				m1=0;
				n1=0;
				l1=0;
				CreateVar(1,"d",  &m1, &n1, &l1);

				LhsVar(1)=1;
				C2F(putlhsvar)();
				return 0;
			}
			else
			{
				Output=(char*)malloc((strlen(ReturnString)+1)*sizeof(char));
				wsprintf(Output,"%s",ReturnString);
			}
		}
		#endif
	}
	else
	{
		Scierror(999,"parameter must be a string.");
		return 0;
	}
	#ifdef WIN32
	  CreateVarFromPtr( 1, "c",(m1=strlen(Output), &m1),&n1,&Output);
	  LhsVar(1)=1;
	#else
	  sciprint("Only for Windows.\n");
	  LhsVar(0)=0;
	#endif
	
	C2F(putlhsvar)();
	if (Output) {free(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/