/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
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

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "realtime.h"

#ifdef _MSC_VER
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>

#ifndef __GNUC__
#define inline
#endif
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static double simulation_start = 0;
static double simulation_scale = 0;
static __int64 realtime_start = 0;
static int simulation_doinit = 1;
union
{
    FILETIME ftFileTime;
    __int64  ftInt64;
} ftRealTime;
#else
static double simulation_start = 0;
static double simulation_scale = 0;
static unsigned long long realtime_start = 0;
static int simulation_doinit = 1;
#endif

/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static inline unsigned long long TIME2ULL(struct timeval t)
{
    unsigned long long result;
    result = (unsigned long long)t.tv_sec * 1000000 + t.tv_usec;
    return result;
}
#endif
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static inline struct timeval ULL2TIME(unsigned long long t)
{
    struct timeval result;
    result.tv_sec = t / 1000000;
    result.tv_usec = t % 1000000;
    return result;
}
#endif
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
int C2F(realtimeinit)(double *t, double *scale)
{
    struct timeval now;

    gettimeofday(&now, 0);
    realtime_start = TIME2ULL(now);
    simulation_doinit = 1;
    simulation_scale = *scale;
    return 0;
}
#endif
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
int C2F(realtime)(double *t)
{
    struct timeval now;
    unsigned long long realtime_diff;
    double simulation_diff;
    long long delay;

    if (simulation_doinit)
    {
        simulation_doinit = 0;
        simulation_start = *t;
    }
    gettimeofday(&now, 0);
    realtime_diff = TIME2ULL(now) - realtime_start;
    simulation_diff = (*t - simulation_start) * simulation_scale;
    delay = (long long)(simulation_diff * 1000000) - realtime_diff;

    if (delay > 0)
    {
        struct timeval d ;
        d = ULL2TIME(delay);
        select(0, 0, 0, 0, &d);
    }
    return 0;
}
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
int C2F(realtimeinit)(double *t, double *scale)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ftRealTime.ftFileTime);

    realtime_start = ftRealTime.ftInt64;
    simulation_doinit = 1;
    simulation_scale = *scale;
    return 0;
}
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
int C2F(realtime)(double *t)
{
    __int64 realtime_diff;
    double simulation_diff;
    __int64 delay;
    SYSTEMTIME st;

    if (simulation_doinit)
    {
        simulation_doinit = 0;
        simulation_start = *t;
    }
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ftRealTime.ftFileTime);

    realtime_diff = ftRealTime.ftInt64  - realtime_start;
    simulation_diff = (*t - simulation_start) * simulation_scale;
    delay = (__int64)(simulation_diff * 10000000) - realtime_diff;

    if (delay > 0)
    {
        Sleep((DWORD)(delay / 10000));
    }
    return 0;
}
#endif
/*--------------------------------------------------------------------------*/
