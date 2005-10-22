/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intmcisendstring.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
	#include <windows.h>
	#pragma comment(lib, "winmm.lib")
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(intmcisendstring) _PARAMS((char *fname))
{
#if WIN32
	mcisendstringWindowsfunction(fname);
#else
	mcisendstringUnixfunction(fname);
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
#if WIN32
int mcisendstringWindowsfunction _PARAMS((char *fname))
{int m1,n1,l1;

	char *Output=NULL;
	int *BoolOutput=NULL;
	int *CodeOutput=NULL;

	CheckRhs(1,1);
    CheckLhs(1,3);

	if (GetType(1) == sci_strings)	
	{
		MCIERROR Error;
		char *MCICommand=NULL;
		char ReturnString[2048];

		GetRhsVar(1,"c",&m1,&n1,&l1);
		MCICommand=cstk(l1);

		Error=mciSendString(MCICommand,(LPSTR)ReturnString,sizeof(ReturnString),NULL);

		BoolOutput=(int*)MALLOC(sizeof(int));
		CodeOutput=(int*)MALLOC(sizeof(int));

		*BoolOutput=(int)0;
		*CodeOutput=(int)Error;
		if (Error)
		{
			char ErrorText[128];
			*BoolOutput=(int)FALSE;
			
			if ( mciGetErrorString(Error,ErrorText,sizeof(ErrorText)) == FALSE )
			{
				wsprintf(ErrorText,"%s","Unkown MCI error");
			}

			Output=(char*)MALLOC((strlen(ErrorText)+1)*sizeof(char));
			wsprintf(Output,"%s",ErrorText);
		}
		else
		{
			*BoolOutput=(int)TRUE;
			Output=(char*)MALLOC((strlen("OK")+1)*sizeof(char));
			wsprintf(Output,"%s","OK");
		}

		n1=1;

		if ( Lhs == 1 )
		{
			CreateVarFromPtr(1, "b", &n1, &n1, &BoolOutput);
			LhsVar(1)=1;
		}
		else
		if ( Lhs == 2 )
		{
			CreateVarFromPtr(1, "b", &n1, &n1, &BoolOutput);
			LhsVar(1)=1;

			CreateVarFromPtr(2, "i", &n1, &n1, &CodeOutput);
			LhsVar(2)=2;
		}
		else /* Lhs == 3 */
		{
			CreateVarFromPtr(1, "b", &n1, &n1, &BoolOutput);
			LhsVar(1)=1;

			CreateVarFromPtr(2, "i", &n1, &n1, &CodeOutput);
			LhsVar(2)=2;

			CreateVarFromPtr( 3, "c",(m1=strlen(Output), &m1),&n1,&Output);
			LhsVar(3)=3;
		}
		
		C2F(putlhsvar)();

		if (Output) {FREE(Output);Output=NULL;}
		if (BoolOutput) {FREE(BoolOutput);BoolOutput=NULL;}
		if (CodeOutput) {FREE(CodeOutput);Output=NULL;}

		return 0;
	}
	else
	{
		Scierror(999,"parameter must be a string.");
		return 0;
	}
	return 0;
}
#else
/*-----------------------------------------------------------------------------------*/
int mcisendstringUnixfunction _PARAMS((char *fname))
{
	int m1,n1,l1;

	char *Output=NULL;
	int *BoolOutput=NULL;
	int *CodeOutput=NULL;

	CheckRhs(1,1);
    CheckLhs(1,3);

	if (GetType(1) == sci_strings)	
	{
		BoolOutput=(int*)MALLOC(sizeof(int));
		CodeOutput=(int*)MALLOC(sizeof(int));

		*BoolOutput=(int)0;
		*CodeOutput=(int)0;

		Output=(char*)MALLOC((strlen("Only for Windows.\n")+1)*sizeof(char));
		sprintf(Output,"%s","Only for Windows.\n");

		n1=1;

		if ( Lhs == 1 )
		{
			CreateVarFromPtr(1, "b", &n1, &n1, &BoolOutput);
			LhsVar(1)=1;
		}
		else
		if ( Lhs == 2 )
		{
			CreateVarFromPtr(1, "b", &n1, &n1, &BoolOutput);
			LhsVar(1)=1;

			CreateVarFromPtr(2, "i", &n1, &n1, &CodeOutput);
			LhsVar(2)=2;
		}
		else /* Lhs == 3 */
		{
			CreateVarFromPtr(1, "b", &n1, &n1, &BoolOutput);
			LhsVar(1)=1;

			CreateVarFromPtr(2, "i", &n1, &n1, &CodeOutput);
			LhsVar(2)=2;

			CreateVarFromPtr( 3, "c",(m1=strlen(Output), &m1),&n1,&Output);
			LhsVar(3)=3;
		}
		
		C2F(putlhsvar)();

		if (Output) {FREE(Output);Output=NULL;}
		if (BoolOutput) {FREE(BoolOutput);BoolOutput=NULL;}
		if (CodeOutput) {FREE(CodeOutput);Output=NULL;}

		return 0;
	}
	else
	{
		Scierror(999,"parameter must be a string.");
		return 0;
	}
	return 0;
}
#endif
/*-----------------------------------------------------------------------------------*/
