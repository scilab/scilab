
/* $Id: pvmwin.h,v 1.1 2001/04/26 07:47:12 scilab Exp $ */

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer, 
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

/* cruft file for different compiler specifications 
*  like Watcom, Visual, Borland ... 
*
*  added __fortran stub    06/21/1997 	Fischer
*/

#ifdef WIN32
#define __fortran __stdcall
#endif
#define STRICT
#ifndef __ABSC__
#include <sys/timeb.h>
#include <process.h>
#include <io.h>
#endif
#include <time.h>
#include <winsock.h>
#include <windows.h>

#include <stdio.h>

#ifndef HASSTDLIB
#define HASSTDLIB
#endif

#ifndef SYSVBFUNC
#define SYSVBFUNC
#endif

#ifndef SYSVSTR
#define SYSVSTR
#endif

#ifndef HASERRORVARS
#define HASERRORVARS
#endif

#ifndef NOWAIT3
#define NOWAIT3
#endif

#ifndef NOGETDTBLSIZ
#define NOGETDTBLSIZ
#endif

#ifndef ARCHCLASS 
#define ARCHCLASS "WIN32"
#endif

#ifndef NOUNIXDOM
#define NOUNIXDOM
#endif

#ifndef NOTMPNAM
#define NOTMPNAM
#endif

#ifndef HASERRORVARS
#define HASERRORVARS
#endif
/* sys/param.h doesn't exist: */
#define MAXHOSTNAMELEN 64
#define MAXPATHLEN 64
	   
#ifndef TIME
#define TIME
struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};
#endif


/* to keep the compiler satisfied */
extern int gettimeofday(struct timeval *, struct timezone *);
extern char *MyGetUserName();
extern int kill(int,int, int);
 
extern HANDLE win32_open_file( char *);
extern HANDLE win32_create_file(char *s, int );
extern int win32_close_file(HANDLE);
extern int win32_write_file(HANDLE,char *,int);
extern int win32_read_file(HANDLE,char *, int);
extern int win32_write_socket (SOCKET, const char FAR *, int);
extern int win32_read_socket (SOCKET, char FAR *, int);

extern void Wait(HANDLE id, DWORD *prv);

