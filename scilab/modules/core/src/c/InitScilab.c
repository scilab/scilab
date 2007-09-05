/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#include "InitializeCore.h"
#include "../../../shell/includes/InitializeShell.h"
#include "../../../console/includes/InitializeConsole.h"
#include "../../../tclsci/includes/InitializeTclTk.h"
#include "../../../localization/includes/InitializeLocalization.h"
#include "../../../graphics/includes/graphicModuleLoad.h"
#include "../../../jvm/includes/InitializeJVM.h"
#ifdef _MSC_VER
#include "../../../windows_tools/includes/InitializeWindows_tools.h"
#endif
#include "../../../gui/includes/InitializeGUI.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
int C2F(initscilab)(void)
{
	#ifdef _MSC_VER
	InitializeWindows_tools();
	#endif

	InitializeCore();

        InitializeShell();

	if ( getScilabMode() != SCILAB_NWNI ) 
	{

		InitializeJVM();
		InitializeGUI();
		
                /* create needed data structure if not already created */
		loadGraphicModule() ;
                
                /* Standard mode -> init Java Console */
                if ( getScilabMode() == SCILAB_STD ) 
                  {
                    /* Initialize console: lines... */
                   InitializeConsole();
                  }
	}

	InitializeTclTk();

	InitializeLocalization();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
