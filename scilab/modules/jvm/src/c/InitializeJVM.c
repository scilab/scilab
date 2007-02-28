/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "InitializeJVM.h"
#include "jvms.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_LIBJVM_DLL BOOL InitializeJVM(void)
{
	BOOL bOK=FALSE;
	char *SCIPATH=NULL;

	SCIPATH=getSCIpath();
	StartJVMs(SCIPATH);
	if (SCIPATH) {FREE(SCIPATH);SCIPATH=NULL;}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

