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
#ifdef WITH_TK
#include "../../../tclsci/includes/TerminateTclTk.h"
#endif
#include "../../../gui/includes/TerminateGui.h"
#include "../../../localization/includes/TerminateLocalization.h"
#include "../../../jvm/includes/TerminateJVM.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 

/*-----------------------------------------------------------------------------------*/ 
int ExitScilab(void)
{
	TerminateCorePart1();

	TerminateGraphics();

	TerminateGUI();
  
	#ifdef WITH_TK
	TerminateTclTk();
	#endif

	TerminateLocalization();

#ifdef _MSC_VER 
	if ( getScilabMode() != SCILAB_NWNI ) TerminateJVM();
#endif

	TerminateCorePart2();

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int C2F(sciquit)(void)
{
	int status = 0;
	ExitScilab();
	sci_exit(status) ;
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void sci_exit(int n) 
{
  /* really exit */
  #ifdef sun 
		#ifndef SYSV
  	{
    	char *mode, **out, *in;
    	ieee_flags("clearall","exeption","all", &out);
  	}
		#endif 
  #endif 
  exit(n);
}
/*-----------------------------------------------------------------------------------*/
