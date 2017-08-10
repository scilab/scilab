/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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

#ifndef __TIMER_HXX__
#define __TIMER_HXX__

#ifndef _MSC_VER
#include <sys/time.h>
#else
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#endif
#include <iostream>
#include <iomanip>

class Timer
{

private:
#ifndef _MSC_VER
    int start_hour;
    int start_min;
    int start_sec;
    int start_usec;
#else
    LARGE_INTEGER iStart;
    LARGE_INTEGER iEnd;
    LARGE_INTEGER iFreq;
#endif

public:

    //===========================================================================
    // Return the total time that the timer has been in the "running"
    // state since it was first "started" or last "restarted".  For
    // "short" time periods (less than an hour), the actual cpu time
    // used is reported instead of the elapsed time.

    inline double elapsed_time(bool _bRestart = false)
    {
        // FIXME : Tonio
#ifndef _MSC_VER
        struct timeval tv;
        struct timezone tz;
        struct tm *tm;
        gettimeofday(&tv, &tz);
        tm = localtime(&tv.tv_sec);
        double dblTime = ( 3600000.0 * (tm->tm_hour - start_hour) +
                           60000.0 * (tm->tm_min - start_min) +
                           1000.0 * (tm->tm_sec - start_sec) +
                           (1.0 * (tv.tv_usec - start_usec)) / 1000.0
                         );
#else
        QueryPerformanceCounter(&iEnd);
        double dblTime	= ((iEnd.QuadPart - iStart.QuadPart) * 1000.0) / iFreq.QuadPart;
#endif

        if (_bRestart == true)
        {
            start();
        }
        return dblTime;
    } // timer::elapsed_time
public:
    Timer()
    {
#ifndef _MSC_VER
        start_hour	= 0;
        start_min		= 0;
        start_sec		= 0;
        start_usec	= 0;
#else
        iStart.QuadPart	= 0;
        iEnd.QuadPart		= 0;
        QueryPerformanceFrequency(&iFreq);
#endif
    }

    //===========================================================================
    // Start a timer.  If it is already running, let it continue running.
    // Print an optional message.

    inline void start(const std::wstring _msg = std::wstring(L""))
    {
        // Print an optional message, something like "Starting timer t";
        if (_msg.empty() == false)
        {
            std::wcerr << _msg << std::endl;
        }
#ifndef _MSC_VER
        // FIXME : Tonio

        // Set the start time
        struct timeval tv;
        struct timezone tz;
        struct tm *tm;
        gettimeofday(&tv, &tz);
        tm = localtime(&tv.tv_sec);
        start_hour = tm->tm_hour;
        start_min = tm->tm_min;
        start_sec = tm->tm_sec;
        start_usec = tv.tv_usec;
#else
        QueryPerformanceCounter(&iStart);
#endif
    } // timer::start

    //===========================================================================
    // Stop the timer and print an optional message.

    //===========================================================================
    // Print out an optional message followed by the current timer timing.

    inline double check(const std::wstring& _msg, bool _bRestart = false)
    {
        // Print an optional message, something like "Checking timer t";
        double t = elapsed_time();
        std::streamsize ss = std::cout.precision();
        if (_msg.empty() == false)
        {
            std::wcerr << L"[" << _msg << L"]" << L" : ";
        }
        std::wcerr << L"Elapsed time ["
                   << std::setiosflags(std::ios::fixed)
                   << std::setprecision(3)
                   << t
                   << std::setprecision(ss)
                   << L"] milliseconds"
                   << std::endl;

        if (_bRestart == true)
        {
            start();
        }
        return t;
    } // timer::check

};

//===========================================================================
// Allow timers to be printed to ostreams using the syntax 'os << t'
// for an ostream 'os' and a timer 't'.  For example, "cout << t" will
// print out the total amount of time 't' has been "running".

inline std::wostream& operator<<(std::wostream& os, Timer& t)
{
    os << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << t.elapsed_time() ;
    return os;
}

//===========================================================================
#endif /* !__TIMER_HXX__ */
