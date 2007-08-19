/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "InitializeGUI.h"
#include "scilabmode.h"
#include "setMainWindowTitle.h"
#include "MALLOC.h"
#include "buildMainWindowTitle.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeGUI(void)
{
	BOOL bOK = FALSE;
	if (getScilabMode() == SCILAB_STD)
	{
		char *title = buildMainWindowTitle();
		if (title)
		{
			bOK = setMainWindowTitle(title);
			FREE(title);
			title = NULL;
		}
	}
	else bOK = TRUE;

	return bOK; 
}
/*-----------------------------------------------------------------------------------*/ 
