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
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
int C2F(initscilab)(void)
{
	InitializeCore();

	if ( getScilabMode() != SCILAB_NWNI ) InitializeJVM();

	InitializeTclTk();

	InitializeLocalization();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
