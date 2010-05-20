/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef sun 
	#ifndef SYSV
	#include <sys/ieeefp.h>
	#endif
#endif
#include "sciquit.h"
/*--------------------------------------------------------------------------*/ 
#include "TerminateCore.h"
#include "../../../graphics/includes/TerminateGraphics.h"
#include "dynamic_tclsci.h"
#include "../../../gui/includes/TerminateGui.h"
#include "../../../jvm/includes/TerminateJVM.h"
#ifdef _MSC_VER
#include "../../../windows_tools/includes/TerminateWindows_tools.h"
#include "../../../windows_tools/includes/MutexClosingScilab.h"
#include "../../../windows_tools/src/c/scilab_windows/killScilabProcess.h"
#endif
#include "../../../gui/includes/TerminateGui.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/ 
int ExitScilab(void)
{
	TerminateCorePart1();
  
	if ( getScilabMode() != SCILAB_NWNI ) 
	{
		dynamic_TerminateTclTk();
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
void sciquit(void)
{
#ifdef _MSC_VER
	/* bug 3672 */
	/* Create a Mutex (closing scilab)
	used by files association 
	*/
	createMutexClosingScilab();
#endif

	ExitScilab();

#ifdef sun 
#ifndef SYSV
	char **out;
	ieee_flags("clearall","exception","all", &out);
#endif 
#endif 

#ifdef _MSC_VER
	/* close mutex (closing scilab)
	used by files association 
	*/
	terminateMutexClosingScilab();
#endif
}
/*--------------------------------------------------------------------------*/
