/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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
#ifndef __GETMEMORY__
#define __GETMEMORY__

#ifndef _MSC_VER
#if defined(hpux)
#include <sys/param.h>
#include <sys/pstat.h>
#else
#if defined(__APPLE__)
#import <mach/host_info.h>
#import <mach/mach_host.h>
#import <sys/sysctl.h>
#else
#include <unistd.h>
#endif
#endif
#endif
#if defined(__alpha)	&& !defined(__NetBSD__) && !defined(linux)
#include <sys/sysinfo.h>
#include <machine/hal_sysinfo.h>
#endif
#endif /*GETMEMORY*/

/**
 * Return the size of free memory in megabytes
 * @return the size of free memory
 */
int getfreememory(void);


/**
 * Return the size of memory in megabytes
 * @return the size of memory
 */
int getmemorysize(void);


/*--------------------------------------------------------------------------*/
