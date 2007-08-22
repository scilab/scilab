/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#include "../../../windows_tools/src/c/scilab_windows/console.h"
#endif
#include "version.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
#define DEFAULT_TITLE SCI_VERSION_STRING
/*-----------------------------------------------------------------------------------*/
char *buildMainWindowTitle(void)
{
	char *retTitle = NULL;

	#ifndef _MSC_VER
	retTitle = (char*)MALLOC(sizeof(char)*(strlen(DEFAULT_TITLE)+1));
	if (retTitle) strcpy(retTitle,DEFAULT_TITLE);
	#else
	{
		char CopyNameConsole[MAX_PATH];
		char *FirstOccurence = NULL;
		char *SecondOccurence = NULL;
		char *ScilexConsoleName = getScilexConsoleName();
		strcpy(CopyNameConsole,ScilexConsoleName);
		FirstOccurence = strtok(CopyNameConsole,"("); 
		SecondOccurence= strtok(NULL,"("); 
		if (FirstOccurence && SecondOccurence)
		{
			retTitle = (char*)MALLOC(sizeof(char)*(strlen("%s (%s")+strlen(SCI_VERSION_STRING)+strlen(SecondOccurence)+1));
			wsprintf(retTitle,"%s (%s",SCI_VERSION_STRING,SecondOccurence);
		}
		else
		{
			retTitle = (char*)MALLOC(sizeof(char)*(strlen(DEFAULT_TITLE)+1));
			if (retTitle) strcpy(retTitle,DEFAULT_TITLE);
		}
	}
	#endif
	return retTitle;
}
/*-----------------------------------------------------------------------------------*/
