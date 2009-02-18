/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "LaunchScilabSignal.h"
#include "Thread_Wrapper.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
 __declspec(dllexport) __threadSignal		LaunchScilab;
 __declspec(dllexport) __threadSignalLock	LaunchScilabLock;
#else
 __threadSignal		LaunchScilab;
 __threadSignalLock	LaunchScilabLock;

#endif
/*--------------------------------------------------------------------------*/
void InitializeLaunchScilabSignal(void)
{
	__InitSignal(&LaunchScilab);
	__InitSignalLock(&LaunchScilabLock);
}
/*--------------------------------------------------------------------------*/
