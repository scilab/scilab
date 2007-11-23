/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifdef sun 
	#ifndef SYSV
	#include <sys/ieeefp.h>
	#endif
#endif
#include "sciquit.h"
/*--------------------------------------------------------------------------*/ 
#include "TerminateCore.h"
#include "../../../graphics/includes/TerminateGraphics.h"
#include "../../../tclsci/includes/TerminateTclTk.h"
#include "../../../gui/includes/TerminateGui.h"
#include "../../../jvm/includes/TerminateJVM.h"
#ifdef _MSC_VER
#include "../../../windows_tools/includes/TerminateWindows_tools.h"
#endif
#include "../../../gui/includes/TerminateGui.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/ 
int ExitScilab(void)
{
	TerminateCorePart1();
	
	TerminateGUI();
  
	TerminateTclTk();

	if ( getScilabMode() != SCILAB_NWNI ) 
	{
		TerminateGraphics();
		TerminateGUI();
		TerminateJVM();
	}

	TerminateCorePart2();

	#ifdef _MSC_VER
	TerminateWindows_tools();
	#endif

	return 0;
}
/*--------------------------------------------------------------------------*/ 
void C2F(sciquit)(void)
{
	ExitScilab();
#ifdef sun 
#ifndef SYSV
  	
	char **out;
	ieee_flags("clearall","exeption","all", &out);
  	
#endif 
#endif 
		exit(0);
}
/*--------------------------------------------------------------------------*/
