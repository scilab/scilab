
/* $Id: pvm3.h,v 1.1 2001/04/26 07:47:08 scilab Exp $ */

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
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

/*
 *	pvm3.h
 *
 *	Libpvm3 includes.
 *
$Log: pvm3.h,v $
Revision 1.1  2001/04/26 07:47:08  scilab
Initial revision

 * Revision 1.33  1997/12/30  18:15:32  pvmsrc
 * Oops...  better skip directly to beta 6 instead of beta 5...
 * 	- separate WIN32 release is beta 5...
 * (Spanker=kohl)
 *
 * Revision 1.32  1997/12/29  19:53:51  pvmsrc
 * Upgraded from beta4 to beta5.
 * (Spanker=kohl)
 *
 * Revision 1.31  1997/09/15  20:54:29  pvmsrc
 * Fixed PVM_MAJOR_VERSION and PVM_MINOR_VERSION defines.
 * 	- VER -> VERSION.
 * 	- MAJOR = 3, MINOR = 4 (3.4.*).
 * (Spanker=kohl)
 *
 * Revision 1.30  1997/09/02  13:05:37  pvmsrc
 * Upgraded PVM_VER to 3.4 beta 4.
 * (Spanker=kohl)
 *
 * Revision 1.29  1997/07/09  13:29:36  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.28  1997/07/01  17:19:03  pvmsrc
 * 	The real real beta version number according to Garp.
 *
 * Revision 1.27  1997/07/01  15:54:35  pvmsrc
 * Trying again on the version.
 * 	- just straight 3.4.0.
 *
 * Revision 1.26  1997/07/01  14:17:26  pvmsrc
 * Set version for beta release to 3.4.1.0.
 *
 * Revision 1.25  1997/06/26  19:36:22  pvmsrc
 * 		Added error constant "PvmParentNotSet" so that
 * 		tc_siblings can get sibling tids even if the spawn flags
 * 		had parent as PvmNoParent.
 *
 * Revision 1.24  1997/06/25  19:41:02  pvmsrc
 * WIN32 fixes from Markus.
 *
 * Revision 1.23  1997/06/23  21:08:42  pvmsrc
 * Added new error code "PvmHostrNMstr" / -34.
 * 	- Hoster run on non-master host.
 *
 * Revision 1.22  1997/06/10  16:38:53  pvmsrc
 * Minor text cleanup.
 *
 * Revision 1.21  1997/05/06  18:58:56  pvmsrc
 * Upped version to 3.4.0.16.
 *
 * Revision 1.20  1997/04/17  12:52:28  pvmsrc
 * rename of pvm_mboxinfo() to include word "get"
 *
 * Revision 1.19  1997/04/10  20:09:57  pvmsrc
 * Fixed prototype for pvm_mboxinfo().
 * 	- pvm_getnames() is history.
 *
 * Revision 1.18  1997/04/10  19:34:57  pvmsrc
 * Added new pvmmboxinfo structure.
 * 	- for pvm_mboxinfo().
 *
 * Revision 1.17  1997/04/09  20:21:51  pvmsrc
 * Added PvmMboxWaitForInfo/PVMMBOXWAITFORINFO constants.
 *
 * Revision 1.16  1997/04/07  20:50:48  pvmsrc
 * pvm_addmhf() protoglarp set to new parameter interface
 *
 * Revision 1.15  1997/04/03  18:19:02  pvmsrc
 * Added a define for PvmBaseContext - this is 3.3 compatible context.
 * codes that don't ever set their context should operate in this context.
 *
 * Revision 1.14  1997/04/01  16:46:09  pvmsrc
 * Fixed protos for pvm_putinfo(), pvm_recvinfo() (formerly pvm_getinfo()).
 *
 * Revision 1.13  1997/03/31  21:38:05  pvmsrc
 * Replaced original mbox constants with new syntax:
 * 	- gone:  PvmExclusive, PvmFirstAvail, PvmLock, PvmSticky.
 * 	- new:  PvmMboxDefault, PvmMboxPersistent, PvmMboxMultiInstance,
 * 		PvmMboxOverWritable, PvmMboxFirstAvail, PvmMboxReadAndDelete.
 * 	- fortran equivalents.
 * 	- checked in for sscott.
 *
 * Revision 1.12  1997/03/26  21:51:55  pvmsrc
 * Added "PvmNoSpawnParent" spawn option constant.
 * 	- PVMNOSPAWNPARENT for Fortran.
 *
 * Revision 1.11  1997/03/07  14:41:10  pvmsrc
 * PVM patches from the base 3.3.10 to 3.3.11 versions where applicable.
 * Originals by Bob Manchek. Altered by Graham Fagg where required.
 *
 * added pvm_export / pvm_unexport to user API.
 *
 * Revision 1.10  1997/03/04  19:44:11  pvmsrc
 * Added system message mailbox class name #defines:
 * 	- PVMHOSTERCLASS, PVMTASKERCLASS, PVMTRACERCLASS, PVMRMCLASS.
 *
 * Revision 1.9  1997/02/17  18:47:51  pvmsrc
 * Oops...  must pass ptr to tids array for pvm_getnoresets().
 *
 * Revision 1.8  1997/02/17  16:30:11  pvmsrc
 * Added proto for new pvm_getnoresets() routine.
 * 	- note:  not an official interface routine...
 *
 * Revision 1.7  1997/02/13  23:12:41  pvmsrc
 * Added PVMNORESETCLASS class name for PvmNoReset.
 *
 * Revision 1.6  1997/02/13  23:05:22  pvmsrc
 * Added new PvmNoReset setopt/getopt constant.
 *
 * Revision 1.5  1997/01/28  19:14:20  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.4  1996/10/24  19:33:48  pvmsrc
 * Modified for New Tracing Facility:
 * 	- added user-defined message encoding, PvmDataTrace.
 * 	- added new option constants:
 * 		PvmTraceBuffer, PvmTraceOptions,
 * 		PvmSelfTraceBuffer & PvmSelfTraceOptions,
 * 	- added PvmTraceFull, PvmTraceTime & PvmTraceCount for trace opts.
 * 	- added function prototype for new pvm_reg_tracer() routine.
 *
 * Revision 1.3  1996/10/11  17:01:52  pvmsrc
 * Replaced references to pvm_remove() with pvm_delinfo():  renamed.
 *
 * Revision 1.2  1996/10/08  18:30:30  pvmsrc
 * Renamed routines:
 * 	- pvm_put() -> pvm_putinfo().
 * 	- pvm_get() -> pvm_getinfo().
 *
 * Revision 1.1  1996/09/23  21:06:31  pvmsrc
 * Initial revision
 *
 */

#ifndef	_PVM3_H_

#define	_PVM3_H_

/*
*	Release version
*/

#define	PVM_VER				"3.4.beta6"
#define	PVM_MAJOR_VERSION	3
#define	PVM_MINOR_VERSION	4

#ifndef WIN32
#include	<sys/time.h>
#else
#include "pvmwin.h"
#ifndef __ABSC__
#include <sys/timeb.h>
#endif
#include <time.h>
#endif

/*
*	Data packing styles for pvm_initsend()
*/

#define	PvmDataDefault	0	/* XDR encoding */
#define	PvmDataRaw		1	/* Raw data copy */
#define	PvmDataInPlace	2	/* Raw data, left in place */
#define	PvmDataFoo		PvmDataDefault	/* Internal use */
#define	PvmDataTrace	4	/* User-Defined Trace Encoding */

/*
*	pvm_spawn options
*/

#define	PvmTaskDefault	0
#define	PvmTaskHost		1	/* specify host */
#define	PvmTaskArch		2	/* specify architecture */
#define	PvmTaskDebug	4	/* start task in debugger */
#define	PvmTaskTrace	8	/* process generates trace data */
/* for MPP ports */
#define	PvmMppFront		16	/* spawn task on service node */
#define	PvmHostCompl	32	/* complement host set */
/* for parent-less spawning */
#define PvmNoSpawnParent	64

/*
*	pvm_notify kinds
*/

#define	PvmTaskExit		1	/* on task exit */
#define	PvmHostDelete	2	/* on host fail/delete */
#define	PvmHostAdd		3	/* on host startup */
#define	PvmRouteAdd		4	/* new task-task route opened */
#define	PvmRouteDelete	5	/* task-task route closed */

/* flags combined with notify kind */

#define	PvmNotifyCancel	256	/* cancel (complete immediately) notifies */

/*
*	for pvm_setopt and pvm_getopt
*/

#define	PvmRoute			1	/* routing policy */
#define		PvmDontRoute		1	/* don't allow direct task-task links */
#define		PvmAllowDirect		2	/* allow direct links, but don't request */
#define		PvmRouteDirect		3	/* request direct links */
#define	PvmDebugMask		2	/* debugmask */
#define	PvmAutoErr			3	/* auto error reporting */
#define	PvmOutputTid		4	/* stdout destination for children */
#define	PvmOutputCode		5	/* stdout message tag */
#define	PvmTraceTid			6	/* trace destination for children */
#define	PvmTraceCode		7	/* trace message tag */
#define	PvmTraceBuffer		8	/* trace buffering for children */
#define	PvmTraceOptions		9	/* trace options for children */
#define		PvmTraceFull		1	/* do full trace events */
#define		PvmTraceTime		2	/* only do PVM routine timings */
#define		PvmTraceCount		3	/* only do PVM routine profiling */
#define	PvmFragSize			10	/* message fragment size */
#define	PvmResvTids			11	/* allow reserved message tids and codes */
#define	PvmSelfOutputTid	12	/* stdout destination for task */
#define	PvmSelfOutputCode	13	/* stdout message tag */
#define	PvmSelfTraceTid		14	/* trace destination for task */
#define	PvmSelfTraceCode	15	/* trace message tag */
#define	PvmSelfTraceBuffer	16	/* trace buffering for task */
#define	PvmSelfTraceOptions	17	/* trace options for task */
#define	PvmShowTids			18	/* pvm_catchout prints task ids with output */
#define	PvmPollType			19	/* shared memory wait method */
#define		PvmPollConstant		1
#define		PvmPollSleep		2
#define	PvmPollTime			20	/* time before sleep if PvmPollSleep */
#define	PvmOutputContext	21	/* stdout message context */
#define	PvmTraceContext		22	/* trace message context */
#define	PvmSelfOutputContext	23	/* stdout message context */
#define	PvmSelfTraceContext	24	/* trace message context */
#define PvmNoReset			25	/* do not kill task on reset */

/*
*	for pvm_[sg]ettmask
*/

#define	PvmTaskSelf		0	/* this task */
#define	PvmTaskChild	1	/* (future) child tasks */

/*
*	Need to have PvmBaseContext defined
*/

#define PvmBaseContext	0

/*
*	for message mailbox operations: pvm_putinfo and pvm_recvinfo
*/

#define	PvmMboxDefault			0	/* put: single locked instance */
									/* recv: 1st entry */
									/* start w/index=0 */
#define	PvmMboxPersistent		1	/* entry remains after owner exit */
#define	PvmMboxMultiInstance	2	/* multiple entries in class */
#define	PvmMboxOverWritable		4	/* can write over this entry */
#define	PvmMboxFirstAvail		8	/* select 1st index >= specified */
#define	PvmMboxReadAndDelete	16	/* atomic read / delete */
									/* requires read & delete rights */
#define	PvmMboxWaitForInfo		32	/* for blocking recvinfo */


#define PVMNORESETCLASS		"###_PVM_NO_RESET_###"

#define PVMHOSTERCLASS		"###_PVM_HOSTER_###"

#define PVMTASKERCLASS		"###_PVM_TASKER_###"

#define PVMTRACERCLASS		"###_PVM_TRACER_###"

#define PVMRMCLASS			"###_PVM_RM_###"

/*
*	Libpvm error codes
*/

#define	PvmOk			0	/* Success */
#define	PvmBadParam		-2	/* Bad parameter */
#define	PvmMismatch		-3	/* Parameter mismatch */
#define	PvmOverflow		-4	/* Value too large */
#define	PvmNoData		-5	/* End of buffer */
#define	PvmNoHost		-6	/* No such host */
#define	PvmNoFile		-7	/* No such file */
#define	PvmDenied		-8	/* Permission denied */
#define	PvmNoMem		-10	/* Malloc failed */
#define	PvmBadMsg		-12	/* Can't decode message */
#define	PvmSysErr		-14	/* Can't contact local daemon */
#define	PvmNoBuf		-15	/* No current buffer */
#define	PvmNoSuchBuf	-16	/* No such buffer */
#define	PvmNullGroup	-17	/* Null group name */
#define	PvmDupGroup		-18	/* Already in group */
#define	PvmNoGroup		-19	/* No such group */
#define	PvmNotInGroup	-20	/* Not in group */
#define	PvmNoInst		-21	/* No such instance */
#define	PvmHostFail		-22	/* Host failed */
#define	PvmNoParent		-23	/* No parent task */
#define	PvmNotImpl		-24	/* Not implemented */
#define	PvmDSysErr		-25	/* Pvmd system error */
#define	PvmBadVersion	-26	/* Version mismatch */
#define	PvmOutOfRes		-27	/* Out of resources */
#define	PvmDupHost		-28	/* Duplicate host */
#define	PvmCantStart	-29	/* Can't start pvmd */
#define	PvmAlready		-30	/* Already in progress */
#define	PvmNoTask		-31	/* No such task */
#define	PvmNotFound		-32	/* Not Found */
#define	PvmExists		-33	/* Already exists */
#define	PvmHostrNMstr	-34	/* Hoster run on non-master host */
#define	PvmParentNotSet	-35	/* Spawning parent set PvmNoSpawnParent */
/*
*	these are going away in the next version.
*	use the replacements
*/
#ifdef	PVM33COMPAT
#define	PvmNoEntry		-32	/* No such entry */
#define	PvmDupEntry		-33	/* Duplicate entry */
#endif

/*
*	Data types for pvm_reduce(), pvm_psend(), pvm_precv()
*/

#define	PVM_STR			0	/* string */
#define	PVM_BYTE		1	/* byte */
#define	PVM_SHORT		2	/* short */
#define	PVM_INT			3	/* int */
#define	PVM_FLOAT		4	/* real */
#define	PVM_CPLX		5	/* complex */
#define	PVM_DOUBLE		6	/* double */
#define	PVM_DCPLX		7	/* double complex */
#define	PVM_LONG		8	/* long integer */
#define	PVM_USHORT		9	/* unsigned short int */
#define	PVM_UINT		10	/* unsigned int */
#define	PVM_ULONG		11	/* unsigned long int */

/*
*	returned by pvm_config()
*/

struct pvmhostinfo {
	int hi_tid;			/* pvmd tid */
	char *hi_name;		/* host name */
	char *hi_arch;		/* host arch */
	int hi_speed;		/* cpu relative speed */
	int hi_dsig;		/* data signature */
};

/*
*	returned by pvm_tasks()
*/

struct pvmtaskinfo {
	int ti_tid;				/* task id */
	int ti_ptid;			/* parent tid */
	int ti_host;			/* pvmd tid */
	int ti_flag;			/* status flags */
	char *ti_a_out;			/* a.out name */
	int ti_pid;				/* task (O/S dependent) process id */
};

/*
*	for pvm_getminfo(), pvm_setminfo()
*/

struct pvmminfo {
	int len;				/* message length */
	int ctx;				/* context */
	int tag;				/* tag */
	int wid;				/* wait id */
	int enc;				/* encoding */
	int crc;				/* crc checksum */
	int src;				/* source tid */
	int dst;				/* destination tid */
};

/*
*	returned by pvm_getmboxinfo()
*/

struct pvmmboxinfo {
	char *mi_name;			/* class name */
	int mi_nentries;		/* # of entries for this class */
	int *mi_indices;		/* mbox entry indices */
	int *mi_owners;			/* mbox entry owner tids */
	int *mi_flags;			/* mbox entry flags */
};

#ifdef __ProtoGlarp__
#undef __ProtoGlarp__
#endif
#if defined(__STDC__) || defined(__cplusplus)
#define __ProtoGlarp__(x) x
#else
#define __ProtoGlarp__(x) ()
#endif

#ifdef __cplusplus
extern "C" {
#endif

int pvm_addhosts	__ProtoGlarp__(( char **, int, int * ));
int pvm_addmhf		__ProtoGlarp__(( int, int, int, int (*) (int) ));
int pvm_archcode	__ProtoGlarp__(( char * ));
int pvm_barrier		__ProtoGlarp__(( char *, int ));
int pvm_bcast		__ProtoGlarp__(( char *, int ));
int pvm_bufinfo		__ProtoGlarp__(( int, int *, int *, int * ));
#if defined(EOF)
int pvm_catchout	__ProtoGlarp__(( FILE * ));
#endif
int pvm_config		__ProtoGlarp__(( int *, int *,
										struct pvmhostinfo ** ));
int pvm_delhosts	__ProtoGlarp__(( char **, int, int * ));
int pvm_delinfo		__ProtoGlarp__(( char *, int, int ));
int pvm_delmhf		__ProtoGlarp__(( int ));
int pvm_exit		__ProtoGlarp__(( void ));
int pvm_export		__ProtoGlarp__(( char * ));
int pvm_freebuf		__ProtoGlarp__(( int ));
int pvm_freecontext	__ProtoGlarp__(( int ));
int pvm_gather		__ProtoGlarp__(( void*, void*,
										int, int, int, char*, int));
int pvm_getcontext	__ProtoGlarp__(( void ));
int pvm_getfds		__ProtoGlarp__(( int ** ));
int pvm_getinst		__ProtoGlarp__(( char *, int ));
int pvm_getminfo	__ProtoGlarp__(( int, struct pvmminfo * ));
int pvm_getnoresets	__ProtoGlarp__(( int **, int * ));
int pvm_getopt		__ProtoGlarp__(( int ));
int pvm_getrbuf		__ProtoGlarp__(( void ));
int pvm_getsbuf		__ProtoGlarp__(( void ));
int pvm_gettid		__ProtoGlarp__(( char *, int ));
int pvm_gsize		__ProtoGlarp__(( char * ));
int pvm_halt		__ProtoGlarp__(( void ));
int pvm_hostsync	__ProtoGlarp__(( int, struct timeval *,
										struct timeval * ));
int pvm_initsend	__ProtoGlarp__(( int ));
int pvm_joingroup	__ProtoGlarp__(( char * ));
int pvm_kill		__ProtoGlarp__(( int ));
int pvm_lvgroup		__ProtoGlarp__(( char * ));
int pvm_getmboxinfo	__ProtoGlarp__(( char *, int *,
										struct pvmmboxinfo ** ));
int pvm_mcast		__ProtoGlarp__(( int *, int, int ));
int pvm_mkbuf		__ProtoGlarp__(( int ));
int pvm_mstat		__ProtoGlarp__(( char * ));
int pvm_mytid		__ProtoGlarp__(( void ));
int pvm_newcontext	__ProtoGlarp__(( void ));
int pvm_notify		__ProtoGlarp__(( int, int,
										int, int * ));
int pvm_nrecv		__ProtoGlarp__(( int, int ));
int pvm_packf		__ProtoGlarp__(( const char *, ... ));
int pvm_parent		__ProtoGlarp__(( void ));
int pvm_perror		__ProtoGlarp__(( char * ));
int pvm_pkbyte		__ProtoGlarp__(( char *, int, int ));
int pvm_pkcplx		__ProtoGlarp__(( float *, int, int ));
int pvm_pkdcplx		__ProtoGlarp__(( double *, int, int ));
int pvm_pkdouble	__ProtoGlarp__(( double *, int, int ));
int pvm_pkfloat		__ProtoGlarp__(( float *, int, int ));
int pvm_pkint		__ProtoGlarp__(( int *, int, int ));
int pvm_pklong		__ProtoGlarp__(( long *, int, int ));
int pvm_pkshort		__ProtoGlarp__(( short *, int, int ));
int pvm_pkstr		__ProtoGlarp__(( char * ));
int pvm_pkuint		__ProtoGlarp__(( unsigned int *, int, int ));
int pvm_pkulong		__ProtoGlarp__(( unsigned long *, int, int ));
int pvm_pkushort	__ProtoGlarp__(( unsigned short *, int, int ));
int pvm_precv		__ProtoGlarp__(( int, int,
									void *, int, int,
									int *, int *, int * ));
int pvm_probe		__ProtoGlarp__(( int, int ));
int pvm_psend		__ProtoGlarp__(( int, int,
									void *, int, int ));
int pvm_pstat		__ProtoGlarp__(( int ));
int pvm_putinfo		__ProtoGlarp__(( char *, int, int ));
int pvm_recv		__ProtoGlarp__(( int, int ));
int (*pvm_recvf		__ProtoGlarp__(( int (*)(int, int, int) )) )();
int pvm_recvinfo	__ProtoGlarp__(( char *, int, int ));
int pvm_reduce		__ProtoGlarp__(( void (*)(int*, void*, void*, int*, int*),
									void *, int,
									int, int, char *,
									int ));

/*
*	Predefined pvm_reduce functions
*/
void PvmMax			__ProtoGlarp__(( int *, void *, void *,
									int *, int * ));
void PvmMin			__ProtoGlarp__(( int *, void *, void *,
									int *, int * ));
void PvmSum			__ProtoGlarp__(( int *, void *, void *,
									int *, int * ));
void PvmProduct		__ProtoGlarp__(( int *, void *, void *,
									int *, int * ));

int pvm_reg_hoster	__ProtoGlarp__(( void ));
int pvm_reg_rm		__ProtoGlarp__(( struct pvmhostinfo ** ));
int pvm_reg_tasker	__ProtoGlarp__(( void ));
int pvm_reg_tracer	__ProtoGlarp__(( int, int, int, int, char*,
										int, int ));
int pvm_scatter		__ProtoGlarp__(( void*, void*,
										int, int, int, char*, int));
int pvm_send		__ProtoGlarp__(( int, int ));
int pvm_sendsig		__ProtoGlarp__(( int, int ));
int pvm_setcontext	__ProtoGlarp__(( int ));
int pvm_setminfo	__ProtoGlarp__(( int, struct pvmminfo * ));
int pvm_setopt		__ProtoGlarp__(( int, int ));
int pvm_setrbuf		__ProtoGlarp__(( int ));
int pvm_setsbuf		__ProtoGlarp__(( int ));
int pvm_siblings	__ProtoGlarp__(( int ** ));
int pvm_spawn		__ProtoGlarp__(( char *, char **, int,
										char *, int, int * ));
int pvm_start_pvmd	__ProtoGlarp__(( int, char **, int ));
int pvm_tasks		__ProtoGlarp__(( int, int *,
										struct pvmtaskinfo ** ));
int pvm_tickle		__ProtoGlarp__(( int, int *,
										int *, int * ));
int pvm_tidtohost	__ProtoGlarp__(( int ));
int pvm_trecv		__ProtoGlarp__(( int, int, struct timeval * ));
int pvm_unexport	__ProtoGlarp__(( char * ));
int pvm_unpackf		__ProtoGlarp__(( const char *, ... ));
int pvm_upkbyte		__ProtoGlarp__(( char *, int, int ));
int pvm_upkcplx		__ProtoGlarp__(( float *, int, int ));
int pvm_upkdcplx	__ProtoGlarp__(( double *, int, int ));
int pvm_upkdouble	__ProtoGlarp__(( double *, int, int ));
int pvm_upkfloat	__ProtoGlarp__(( float *, int, int ));
int pvm_upkint		__ProtoGlarp__(( int *, int, int ));
int pvm_upklong		__ProtoGlarp__(( long *, int, int ));
int pvm_upkshort	__ProtoGlarp__(( short *, int, int ));
int pvm_upkstr		__ProtoGlarp__(( char * ));
int pvm_upkuint		__ProtoGlarp__(( unsigned int *, int, int ));
int pvm_upkulong	__ProtoGlarp__(( unsigned long *, int, int ));
int pvm_upkushort	__ProtoGlarp__(( unsigned short *, int, int ));
char *pvm_version	__ProtoGlarp__(( void ));

/*
*	these are going away in the next version.
*	use the replacements
*/

#ifdef	PVM33COMPAT
int pvm_getmwid		__ProtoGlarp__(( int ));
int pvm_setmwid		__ProtoGlarp__(( int, int ));
int pvm_delete		__ProtoGlarp__(( char *, int ));
int pvm_insert		__ProtoGlarp__(( char *, int, int ));
int pvm_lookup		__ProtoGlarp__(( char *, int, int * ));
#endif

#ifdef __cplusplus
}
#endif

#endif	/*_PVM3_H_*/

