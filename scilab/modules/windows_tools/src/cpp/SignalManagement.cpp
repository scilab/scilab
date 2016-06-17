/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016 - 2016 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <windows.h>
#include <iostream>

extern "C"
{
#include "SignalManagement.h"
}

static VOID CALLBACK kill_process_callback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_TIMER Timer)
{
    std::cerr << "Watchdog timer expired: Scilab killed" << std::endl;
	// use the System Error code: wait operation timed out
    ExitProcess(258);
}

void timeout_process_after(int timeoutDelay)
{
    auto timerid = CreateThreadpoolTimer(kill_process_callback, nullptr, nullptr);

    FILETIME FileDueTime;
    ULARGE_INTEGER ulDueTime;

    // Set the timer to fire in the delay in seconds, relative to the current time
	long long in_seconds = - 10 * 1000 * 1000;
    ulDueTime.QuadPart = timeoutDelay * in_seconds;
    FileDueTime.dwHighDateTime = ulDueTime.HighPart;
    FileDueTime.dwLowDateTime  = ulDueTime.LowPart;

    SetThreadpoolTimer(timerid, &FileDueTime, 0, 0);
}
