/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "InitializeCore.h"
#include "SetScilabEnvironment.h"
#include "getmodules.h"
#include "hashtable_core.h"
#include "sciprint.h"
#include "LoadFunctionsTab.h"
#include "prompt.h"
#include "sciprint.h"
#include "filesmanagement.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
BOOL InitializeCore(void)
{
	BOOL bOK=FALSE;
	int pause=0;

	InitializeScilabFilesList();

	getmodules();

	if ( create_hashtable_scilab_functions(MAXELEMENTFUNCTIONLIST) == 0 ) 
	{
		sciprint(_("Fatal Error : Can't create table for scilab functions.\n"));
		exit(1);
	}

	LoadFunctionsTab();

	/* Set prompt level */
	C2F(setprlev)(&pause);

	return bOK;
}
/*--------------------------------------------------------------------------*/ 

