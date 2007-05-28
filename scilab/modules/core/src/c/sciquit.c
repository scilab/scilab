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
#ifdef WITH_JAVA
#include "../../../jvm/includes/TerminateJVM.h"
#endif
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 

/*-----------------------------------------------------------------------------------*/ 
int ExitScilab(void)
{
	TerminateCorePart1();
	
	TerminateGUI();
  
	TerminateTclTk();

	TerminateLocalization();

	if ( getScilabMode() != SCILAB_NWNI ) 
	{
#ifdef WITH_JAVA
		TerminateJVM();
#endif
		TerminateGraphics();
	}

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
