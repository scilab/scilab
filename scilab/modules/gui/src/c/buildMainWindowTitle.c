/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "buildMainWindowTitle.h"
#ifdef _MSC_VER
#include "../../../windows_tools/src/c/scilab_windows/buildMainWindowTitle_Windows.h"
#endif
#include "version.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
char *buildMainWindowTitle(void)
{
	char *retTitle = NULL;

	#ifndef _MSC_VER
	retTitle = (char*)MALLOC(sizeof(char)*(strlen(SCI_VERSION_STRING)+1));
	if (retTitle) strcpy(retTitle,SCI_VERSION_STRING);
	#else
	retTitle = buildMainWindowTitle_Windows();
	#endif
	return retTitle;
}
/*--------------------------------------------------------------------------*/
