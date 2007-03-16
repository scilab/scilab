/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#include "InitializeCore.h"
#include "../../tclsci/includes/InitializeTclTk.h"
#include "../../localization/includes/InitializeLocalization.h"
#ifdef _MSC_VER
#include "../../jvm/includes/InitializeJVM.h"
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(initscilab)(void)
{
	InitializeCore();

	InitializeTclTk();

	
	InitializeLocalization();
	#ifdef _MSC_VER
	InitializeJVM();
	#endif
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
