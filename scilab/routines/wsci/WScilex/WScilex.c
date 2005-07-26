#include <Windows.h>
#include "stdio.h"
#include "signal.h"

#include "../Messages.h"
#include "../Warnings.h"
#include "../Errors.h"

#include "../win_mem_alloc.h" /* MALLOC */

extern int WINAPI Windows_Main(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow);



int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow)
{
	#define MAXCMDTOKENS 128
	int argc=-1;
	static LPSTR argv[MAXCMDTOKENS];
	char *pFullCmdLine=NULL;
	char *pFullCmdLineTmp=NULL;


	pFullCmdLine=GetCommandLine();
	pFullCmdLineTmp=(char *) MALLOC( sizeof(char)*( strlen(pFullCmdLine)+1 ) );
	strcpy(pFullCmdLineTmp,pFullCmdLine);

	argv[++argc] = strtok (pFullCmdLineTmp, " ");
	
	while (argv[argc] != NULL)
	{
		argv[++argc] = strtok(NULL, " ");
	}

    Windows_Main(hInstance,hPrevInstance,szCmdLine, iCmdShow);
    exit(0);	/* exit(0) rather than return(0) to bypass Cray bug */
    return 0;	/* For compilers that complain of missing return values; */
		/* others will complain that this is unreachable code. */
  }
#ifdef __cplusplus
}
#endif


