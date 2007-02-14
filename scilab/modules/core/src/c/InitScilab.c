/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "InitScilab.h"
#include "InitializeCore.h"
#include "../../tclsci/includes/InitializeTclTk.h"
#ifdef _MSC_VER
#include "../../localization/includes/InitializeLocalization.h"
#include "../../jvm/includes/InitializeJVM.h"
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(initscilab)(void)
{
	InitializeCore();

	InitializeTclTk();

	#ifdef _MSC_VER
	InitializeLocalization();
	InitializeJVM();
	#endif
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
