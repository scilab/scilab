#include <Windows.h>
#include "stdio.h"
#include "signal.h"

#include "../Messages.h"
#include "../Warnings.h"
#include "../Errors.h"
#include "../win_mem_alloc.h" /* MALLOC */

extern int Console_Main(int argc, char **argv);

int main (int argc, char **argv)
{

	#define MAXCMDTOKENS 128
	int argcbis=-1;
	LPSTR argvbis[MAXCMDTOKENS];
	int i=0;
	int FindNW=0;

	for (i=0;i<argc;i++)
	{
		if ( (strcmp(argv[i],"-nw")==0) || (strcmp(argv[i],"-NW")==0) ) FindNW=1;
		if ( (strcmp(argv[i],"-nwni")==0) || (strcmp(argv[i],"-NWNI")==0) ) FindNW=1;
	}

	if (FindNW==0)
	{
		char *nwparam=NULL;
		nwparam=(char*)MALLOC((strlen("-nw")+1)*sizeof(char));
		strcpy(nwparam,"-nw");
		for (i=0;i<argc;i++)
		{
			argvbis[i]=argv[i];
		}
		argvbis[argc]=nwparam;
		argcbis=argc+1;


	}
	else
	{
		for (i=0;i<argc;i++)
		{
			argvbis[i]=argv[i];
		}
		argcbis=argc;
	
	}
    	
    Console_Main(argcbis,argvbis);
    exit(0);	/* exit(0) rather than return(0) to bypass Cray bug */
    return 0;	/* For compilers that complain of missing return values; */
		/* others will complain that this is unreachable code. */
  }
#ifdef __cplusplus
}
#endif



