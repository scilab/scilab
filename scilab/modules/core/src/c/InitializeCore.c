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

#ifdef _MSC_VER
/* Force some definitions (linear_algebra)*/
/* may be not the best place ... */
SCIGSCH_struct C2F(scigsch);
SCISCH_struct C2F(scisch);
SCIZGSCH_union C2F(scizgsch);
#endif
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeCore(void)
{
	BOOL bOK=FALSE;
	int pause=0;

	SciEnv();

	getmodules();

	if ( create_hashtable_scilab_functions(MAXTAB) == 0 ) 
	{
		message_scilab("core_message_128");
		exit(1);
	}

	LoadFunctionsTab();

	/* Set prompt level */
	C2F(setprlev)(&pause);

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

