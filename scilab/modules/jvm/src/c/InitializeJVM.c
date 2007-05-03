/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "InitializeJVM.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "JVM.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeJVM(void)
{
	BOOL bOK=FALSE;
	char *SCIPATH=NULL;

	SCIPATH=getSCIpath();

	bOK=startJVM(SCIPATH);

	if (!bOK)
	{
#ifdef _MSC_VER
		MessageBox(NULL,"Scilab can't open jvm library.","Error",MB_ICONEXCLAMATION|MB_OK);
#else
		printf("\n Error : Scilab can't open jvm library.\n");
#endif
	}

	if (SCIPATH) {FREE(SCIPATH);SCIPATH=NULL;}

	if (!bOK) exit(1);

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

