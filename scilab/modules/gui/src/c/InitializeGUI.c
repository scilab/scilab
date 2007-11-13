/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "InitializeGUI.h"
#include "scilabmode.h"
#include "setMainWindowTitle.h"
#include "MALLOC.h"
#include "buildMainWindowTitle.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeGUI(void)
{
	if (getScilabMode() == SCILAB_STD)
	{
		char *title = buildMainWindowTitle();
		if (title)
		{
			BOOL bOK = setMainWindowTitle(title);
			FREE(title);
			title = NULL;
			return bOK;
		}
	}
	else return TRUE;

	return FALSE; 
}
/*-----------------------------------------------------------------------------------*/ 
