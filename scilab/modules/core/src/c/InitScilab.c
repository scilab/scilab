/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#include "InitializeCore.h"
#include "../../../tclsci/includes/InitializeTclTk.h"
#include "../../../localization/includes/InitializeLocalization.h"
#include "../../../graphics/includes/graphicModuleLoad.h"
#include "scilabmode.h"
#include "../../../jvm/includes/InitializeJVM.h"
#include "../../../history_manager/includes/InitializeHistoryManager.h"
/*-----------------------------------------------------------------------------------*/
int C2F(initscilab)(void)
{
	InitializeCore();

	if ( getScilabMode() != SCILAB_NWNI ) 
	{

		InitializeJVM();
		/* create needed data structure if not already created */
		loadGraphicModule() ;
		InitializeHistoryManager();
		
	}

	InitializeTclTk();

	InitializeLocalization();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
