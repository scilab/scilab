/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <Windows.h>
#include "winopen.h"
#include "cluni0.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
#ifndef FILENAME_MAX
#define FILENAME_MAX 4096
#endif
/*-----------------------------------------------------------------------------------*/
BOOL winopen(char *scilabfilename)
{
	BOOL bOK = FALSE;
	char filename[FILENAME_MAX];
	int out_n = 0;
	long int lout = 0;
	HINSTANCE error = NULL;

	lout = FILENAME_MAX;
	C2F(cluni0)(scilabfilename, filename, &out_n,(int)strlen(scilabfilename),lout);
	error = ShellExecute(NULL, "open", filename, NULL, NULL, SW_SHOWNORMAL);
	if ( error <= (HINSTANCE)32) bOK = FALSE;
	else bOK = TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
