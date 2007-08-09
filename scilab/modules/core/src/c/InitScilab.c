/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#include "InitializeCore.h"
#include "../../../tclsci/includes/InitializeTclTk.h"
#include "../../../localization/includes/InitializeLocalization.h"
#include "../../../graphics/includes/graphicModuleLoad.h"
#include "../../../jvm/includes/InitializeJVM.h"
#ifdef _MSC_VER
#include "../../../windows_tools/includes/InitializeWindows_tools.h"
#endif
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
int C2F(initscilab)(void)
{
	#ifdef _MSC_VER
	InitializeWindows_tools();
	#endif

	InitializeCore();

	if ( getScilabMode() != SCILAB_NWNI ) 
	{

		InitializeJVM();
		/* create needed data structure if not already created */
		loadGraphicModule() ;
	}

	InitializeTclTk();

	InitializeLocalization();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
