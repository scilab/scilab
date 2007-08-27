/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <Windows.h>
#include "buildMainWindowTitle_Windows.h"
#include "MALLOC.h"
#include "version.h"
#include "WndThread.h"
/*-----------------------------------------------------------------------------------*/
char *buildMainWindowTitle_Windows(void)
{
	char *title = NULL;

	title = (char*)MALLOC(sizeof(char)*(strlen("%s (%d)")+strlen(SCI_VERSION_STRING)+10+1));
	if (title) wsprintf(title,"%s (%d)",SCI_VERSION_STRING,getCurrentScilabId());

	return title;
}
/*-----------------------------------------------------------------------------------*/
