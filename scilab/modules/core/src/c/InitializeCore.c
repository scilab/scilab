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
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeCore(void)
{
	BOOL bOK=FALSE;
	int pause=0;

	SciEnv();

	getmodules();

	if ( create_hashtable_scilab_functions(MAXTAB) == 0 ) 
	{
		sciprint("Fatal Error : Can't create table for scilab functions \n");
		exit(1);
	}

	LoadFunctionsTab();

	/* Set prompt level */
	C2F(setprlev)(&pause);

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

