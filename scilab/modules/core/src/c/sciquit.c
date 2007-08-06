/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef sun 
	#ifndef SYSV
	#include <sys/ieeefp.h>
	#endif
#endif
#include "sciquit.h"
/*-----------------------------------------------------------------------------------*/ 
#include "TerminateCore.h"
#include "../../../graphics/includes/TerminateGraphics.h"
#include "../../../tclsci/includes/TerminateTclTk.h"
#include "../../../gui/includes/TerminateGui.h"
#include "../../../localization/includes/TerminateLocalization.h"
#include "../../../jvm/includes/TerminateJVM.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 
int ExitScilab(void)
{
	TerminateCorePart1();
	
	TerminateGUI();
  
	TerminateTclTk();

	TerminateLocalization();

	if ( getScilabMode() != SCILAB_NWNI ) 
	{
		TerminateGraphics();
		TerminateJVM();
	}

	TerminateCorePart2();

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void C2F(sciquit)(void)
{
	int status = 0;
	ExitScilab();
  #ifdef sun 
		#ifndef SYSV
  	
    	char *mode, **out, *in;
    	ieee_flags("clearall","exeption","all", &out);
  	
		#endif 
  #endif 
	exit(0);
}
/*-----------------------------------------------------------------------------------*/
