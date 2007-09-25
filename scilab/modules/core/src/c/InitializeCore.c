/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "InitializeCore.h"
#include "SciEnv.h"
#include "getmodules.h"
#include "hashtable_core.h"
#include "sciprint.h"
#include "LoadFunctionsTab.h"
#include "prompt.h"
#include "message_scilab.h"
#include "filesmanagement.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeCore(void)
{
	BOOL bOK=FALSE;
	int pause=0;

	InitializeScilabFilesList();

	SciEnv();

	getmodules();

	if ( create_hashtable_scilab_functions(MAXELEMENTFUNCTIONLIST) == 0 ) 
	{
		message_scilab("Fatal Error : Can't create table for scilab functions.");
		exit(1);
	}

	LoadFunctionsTab();

	/* Set prompt level */
	C2F(setprlev)(&pause);

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

