/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "InitializeLocalization.h"
#include "localization.h"
#include "loadsavelanguage.h"
#include "loadhashtableslocalization.h"
#include "tableslanguages.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeLocalization(void)
{
	BOOL bOK=FALSE;

	if ( InitializeHashTableScilabErrors() && InitializeHashTableScilabMessages() && InitializeHashTableScilabMenus() )
	{
		LoadHashTablesLocalization(SCILABDEFAULTLANGUAGE);
		loadlanguagepref();
	}
	else
	{
		#ifdef _MSC_VER
		MessageBox(NULL,"Problem(s) in Localization module.\nScilab doesn't work.","Error", MB_ICONSTOP | MB_OK); 
		#else
		printf("\nError : Problem(s) in Localization module.\nScilab doesn't work.\n");
		#endif
		exit(1);
	}

	bOK=TRUE;
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

