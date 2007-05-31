/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "getcommandlineargs.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#define MAXCMDTOKENS 256
/*-----------------------------------------------------------------------------------*/ 
static int scilab_nbargs = 0;
static char *scilab_argv[MAXCMDTOKENS];
/*-----------------------------------------------------------------------------------*/ 
char ** getCommandLineArgs(int *nbargs)
{
	char **argv = NULL;
	int i = 0;
	*nbargs = scilab_nbargs;

	argv = (char **)MALLOC(sizeof(char*)*scilab_nbargs);

	for (i = 0;i < scilab_nbargs;i++)
	{
		argv[i] = (char *)MALLOC(sizeof(char)*(strlen(scilab_argv[i])+1));
		strcpy(argv[i],scilab_argv[i]);
	}
	return argv;
}
/*-----------------------------------------------------------------------------------*/ 
int setCommandLineArgs(char **argv, int nbargs)
{
	int i = 0;

	if (nbargs >= MAXCMDTOKENS) nbargs = MAXCMDTOKENS;

	for (i=0;i<nbargs;i++)
	{
		scilab_argv[i] = argv[i];
	}
	scilab_nbargs = nbargs;
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
