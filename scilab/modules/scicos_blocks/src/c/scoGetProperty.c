/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
/**
\file scoGetProperty.c
\author Benoit Bayol
\version 1.0
\date September 2006 - January 2007
\brief Source code of all accessors for ScopeMemory structure
*/
/*--------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "HandleManagement.h" /* sciGetPointerFromHandle */
#include "scoGetProperty.h"
/*--------------------------------------------------------------------------*/ 
//** Added for real time functions 
//**
//** This function return back the system time in seconds with a resolution, on my Linux machine, of 
//** one micro seconds. It is not important if the time is the "asbolute" Linux time, the uptime, or
//** how many seconds after the last Bill Gates birthday, becase the use is strictly differential. 
//** For our application, any resolution better than one milli second is accetable.
//** Due to finite precision (53 bit, 16 decimal, this rapresentation fail the minimum require accuracy
//** (one millisecond) after 317,097 years (enough, I suppose).
//** 
//** for the Windows version 
//**

//** -------------------- LINUX VERSION ------------------------------------------------------------------
#ifndef _MSC_VER

#ifdef HAVE_CLOCK_GETTIME

#include <time.h>
#include <sched.h>

#define NSEC_PER_SEC    1000000000

//** BEWARE : this code is higly LINUX specific ! 

/* the struct timespec consists of nanoseconds
* and seconds. if the nanoseconds are getting
* bigger than 1000000000 (= 1 second) the
* variable containing seconds has to be
* incremented and the nanoseconds decremented
* by 1000000000.
*/

double scoGetRealTime(void) 
{
	struct timespec t_crt      ; //** the current real time as "timespec" (32+32 bit) data structure  

	clock_gettime(0, &t_crt); /* get current time */
	return (double) t_crt.tv_sec + (double)1.0E-9 * (double) t_crt.tv_nsec; //** the current real time in seconds as double (52 bit resolution) 
}
#else
/* not clock gettime on the system√ ... Macosx case? */
#ifdef __APPLE__
/**
* @TODO add the mac os case here
* maybe http://www.wand.net.nz/~smr26/wordpress/2009/01/19/monotonic-time-in-mac-os-x/ 
* could help
*/
double scoGetRealTime(void) 
{
	/* @TODO Fill the blank */
	return 0;
}
#else
#error "Could not find clock_gettime equivalent on this platform, please submit a bug report on http://bugzilla.scilab.org/"
#endif
#endif /* have clock_gettime */
#else
//** -------------------- WINDOWS VERSION ------------------------------------------------------------------ 
#include <windows.h>
#include <winbase.h>

double scoGetRealTime(void) 
{
	ULARGE_INTEGER ctime;
	FILETIME ftFileTime;
	double d_current_real_time ; //** the current real time in seconds as double (52 bit resolution)

	GetSystemTimeAsFileTime(&ftFileTime);     /* Resolution 100 nsec = 0.1 us */
	ctime.LowPart = ftFileTime.dwLowDateTime  ; 
	ctime.HighPart = ftFileTime.dwHighDateTime;

	d_current_real_time = (double)(ctime.QuadPart) * (double) 0.1E-6 ;
	return d_current_real_time ;
}

#endif
//** ------------------- Ends of the add ons for real time function ----------------------------------------
//** -------- Ends of the add ons for real time function -------------
/*--------------------------------------------------------------------------*/ 
scoLongInteger scoGetHandleScopeWindow(ScopeMemory * pScopeMemory)
{
	return pScopeMemory->hScopeWindow;
}
/*--------------------------------------------------------------------------*/ 
scoLongInteger scoGetHandleAxes(ScopeMemory * pScopeMemory, int i)
{
	return pScopeMemory->hAxes[i];
}
/*--------------------------------------------------------------------------*/ 
scoLongInteger scoGetHandleShortDraw(ScopeMemory * pScopeMemory, int i, int j)
{
	return pScopeMemory->hShortDraw[i][j];
}
/*--------------------------------------------------------------------------*/ 
scoLongInteger scoGetHandleLongDraw(ScopeMemory * pScopeMemory, int i, int j)
{
	return pScopeMemory->hLongDraw[i][j];
}
/*--------------------------------------------------------------------------*/ 
scoGraphicalObject scoGetPointerScopeWindow(ScopeMemory * pScopeMemory)
{
	return  sciGetPointerFromHandle(scoGetHandleScopeWindow(pScopeMemory));
}
/*--------------------------------------------------------------------------*/ 
scoGraphicalObject scoGetPointerAxes(ScopeMemory * pScopeMemory, int i)
{
	return sciGetPointerFromHandle(scoGetHandleAxes(pScopeMemory,i));
}
/*--------------------------------------------------------------------------*/ 
scoGraphicalObject scoGetPointerShortDraw(ScopeMemory * pScopeMemory, int i, int j)
{
	return sciGetPointerFromHandle(scoGetHandleShortDraw(pScopeMemory,i,j));
}
/*--------------------------------------------------------------------------*/ 
scoGraphicalObject scoGetPointerLongDraw(ScopeMemory * pScopeMemory, int i, int j)
{
	return sciGetPointerFromHandle(scoGetHandleLongDraw(pScopeMemory,i,j));
}
/*--------------------------------------------------------------------------*/ 
scoInteger scoGetNumberOfSubwin(ScopeMemory * pScopeMemory)
{
	return pScopeMemory->number_of_subwin;
}
/*--------------------------------------------------------------------------*/ 
scoInteger scoGetNumberOfCurvesBySubwin(ScopeMemory * pScopeMemory,int i)
{
	return pScopeMemory->number_of_curves_by_subwin[i];
}
/*--------------------------------------------------------------------------*/ 
scoInteger scoGetPeriodCounter(ScopeMemory * pScopeMemory, int i)
{
	return pScopeMemory->period_counter[i];
}
/*--------------------------------------------------------------------------*/ 
scoInteger scoGetNewDraw(ScopeMemory * pScopeMemory, int i)
{
	return pScopeMemory->new_draw[i];
}
/*--------------------------------------------------------------------------*/ 
scoInteger scoGetLongDrawSize(ScopeMemory * pScopeMemory, int i)
{
	return  pScopeMemory->longdraw_size[i];
}
/*--------------------------------------------------------------------------*/ 
scoInteger scoGetWindowID(ScopeMemory * pScopeMemory)
{
	return pScopeMemory->win_id;
}
/*--------------------------------------------------------------------------*/ 
scoInteger scoGetShortDrawSize(ScopeMemory * pScopeMemory, int i)
{
	return pScopeMemory->shortdraw_size[i];
}
/*--------------------------------------------------------------------------*/ 
scoDouble scoGetPeriod(ScopeMemory * pScopeMemory, int i)
{
	return pScopeMemory->period[i];
}
/*--------------------------------------------------------------------------*/ 
scoInteger scoGetScopeActivation(ScopeMemory * pScopeMemory)
{
	if (pScopeMemory==NULL) 
	{
		return 0;
	}
	else 
	{
		return pScopeMemory->activated;
	}
}
/*--------------------------------------------------------------------------*/ 
