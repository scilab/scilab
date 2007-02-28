/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "InitializeLocalization.h"
#include "localization.h"
#include "loadsavelanguage.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeLocalization(void)
{
	BOOL bOK=FALSE;

	char *SCIPATH=NULL;
	SCIPATH=getSCIpath();
	InitializeHashTableScilabErrors(SCIPATH);

	loadlanguagepref();

	if (SCIPATH) {FREE(SCIPATH);SCIPATH=NULL;}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

