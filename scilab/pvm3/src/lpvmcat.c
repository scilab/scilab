
static char rcsid[] =
	"$Id: lpvmcat.c,v 1.1 2001/04/26 07:47:10 scilab Exp $";

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
 *	lpvmcat.c
 *
 *	Child task output collection.
 *
$Log: lpvmcat.c,v $
Revision 1.1  2001/04/26 07:47:10  scilab
Initial revision

 * Revision 1.8  1997/12/31  20:50:11  pvmsrc
 * Cleaned Up System Message Handlers.
 * 	- current send / recv buffers now saved before invocation of
 * 		message handler functs.
 * 	- removed manual rbf = setrbuf(mid) saving & resetting in
 * 		handlers...
 * (Spanker=kohl)
 *
 * Revision 1.7  1997/06/27  17:32:24  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.6  1997/06/12  20:10:40  pvmsrc
 * Made sure all communications for TC_* task control messages
 * 	use the SYSCTX_TC system context.
 * 	- some messages being sent in default context...  D-Oh...
 *
 * Revision 1.5  1997/04/07  21:09:20  pvmsrc
 * pvm_addmhf() - new paramter interface
 *
 * Revision 1.4  1997/01/28  19:26:24  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/25  13:57:23  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  21:18:29  pvmsrc
 * Moved #include "global.h" to end of #include's for typing.
 * Added #include of new "lpvm.h" in place of explicitly extern-ed
 * 	globals...
 *
 * Revision 1.1  1996/09/23  23:44:13  pvmsrc
 * Initial revision
 *
 * Revision 1.7  1995/11/02  16:08:12  manchek
 * added NEEDSENDIAN switch
 *
 * Revision 1.6  1995/07/28  16:04:05  manchek
 * switch endian includes on flag, not arch name
 *
 * Revision 1.5  1995/05/17  16:17:24  manchek
 * fixed buf in pvmclaimo, copying one byte too few
 *
 * Revision 1.4  1994/12/20  16:35:57  manchek
 * added ShowTids option
 *
 * Revision 1.3  1994/11/07  21:34:49  manchek
 * cast message tag for comparison as int
 *
 * Revision 1.2  1994/10/15  19:10:08  manchek
 * don't segfault if we get a TC_OUTPUT message and pvm_catchout never called
 *
 * Revision 1.1  1994/06/03  20:38:16  manchek
 * Initial revision
 *
 */


#include <stdio.h>
#ifdef NEEDMENDIAN
#include <machine/endian.h>
#endif
#ifdef NEEDENDIAN
#include <endian.h>
#endif
#ifdef NEEDSENDIAN
#include <sys/endian.h>
#endif
#ifndef WIN32
#include <rpc/types.h>
#include <rpc/xdr.h>
#else 
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#endif
#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif
#include <signal.h>
#include <pvm3.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "listmac.h"
#include "bfunc.h"
#include "tvdefs.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "global.h"


/*
*	used to collect output from a task
*/

struct tobuf {
	struct tobuf *o_link, *o_rlink;		/* chain of all tasks in job */
	int o_tid;							/* task id */
	int o_len;							/* length of buf */
	char *o_buf;						/* buffered partial line */
	int o_maxl;							/* space in buf */
	int o_flag;							/* task state */
#define	GOTSPAWN	1
#define	GOTEOF		2
#define	GOTBOTH		(GOTSPAWN|GOTEOF)
#define	TASKSTATE	(GOTSPAWN|GOTEOF)
};


/***************
 **  Globals  **
 **           **
 ***************/


/***************
 **  Private  **
 **           **
 ***************/

static struct tobuf *tobuflist = 0;
static FILE *outlogff = 0;


/**************************
 **  Internal Functions  **
 **                      **
 **************************/

static int pvmclaimo();
static int pvmflusho();

static int
tobuf_init()
{
	int i;
	struct pvmminfo minfo;
	int ictx;

	if (!tobuflist) {
		tobuflist = TALLOC(1, struct tobuf, "obuf");
		BZERO((char*)tobuflist, sizeof(struct tobuf));
		tobuflist->o_link = tobuflist->o_rlink = tobuflist;

		i = pvm_setopt(PvmResvTids, 1);

		BZERO(&minfo, sizeof(minfo));
		minfo.src = -1;
		minfo.ctx = SYSCTX_TC;
		minfo.tag = TC_OUTPUT;
		pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvmclaimo);
		minfo.tag = TC_OUTPUTX;
		pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvmflusho);

		ictx = pvm_setcontext(SYSCTX_TC);
		pvm_notify(PvmTaskExit, TC_OUTPUTX, 1, &pvmmytid);
		pvm_setcontext(ictx);

		pvm_setopt(PvmResvTids, i);
	}
	return 0;
}


static struct tobuf *
tobuf_new(tid)
	int tid;
{
	struct tobuf *op = tobuflist->o_link;
	struct tobuf *op2;

	while (op != tobuflist && op->o_tid < tid)
		op = op->o_link;
	if (op->o_tid != tid) {
		op2 = TALLOC(1, struct tobuf, "tobuf");
		BZERO((char*)op2, sizeof(struct tobuf));
		op2->o_tid = tid;
		LISTPUTBEFORE(op, op2, o_link, o_rlink);
		op = op2;
	}
	return op;
}


static int
tobuf_free(op)
	struct tobuf *op;
{
	LISTDELETE(op, o_link, o_rlink);
	if (op->o_buf)
		PVM_FREE(op->o_buf);
	PVM_FREE(op);
	return 0;
}


static struct tobuf *
tobuf_find(tid)
	int tid;
{
	struct tobuf *op = tobuflist->o_link;

	while (op != tobuflist && op->o_tid < tid)
		op = op->o_link;
	if (op->o_tid == tid)
		return op;
	return (struct tobuf*)0;
}


/*	pvmflusho()
*
*	Called by pvm_exit() to make sure all task output is flushed.
*	If outlogff is nonzero, make sure all child tasks have exited
*	else just toss the output.
*/

static int
pvmflusho()
{
	struct tobuf *op;
	int cc;

	if (outlogff) {
		if (tobuflist && tobuflist->o_link != tobuflist) {
			if (pvmshowtaskid)
				pvmlogerror("child task(s) still running.  waiting...\n");
			while (tobuflist->o_link != tobuflist) {

	/* XXX mroute() does return after ctrl messages received */

				if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
					break;
			}
		}
		pvm_catchout((FILE *)0);
	}

	if (tobuflist) {
		while (tobuflist->o_link != tobuflist)
			tobuf_free(tobuflist->o_link);
		PVM_FREE(tobuflist);
		tobuflist = 0;
	}
	return 0;
}


/*	pvmclaimo()
*
*	We get output from a child* task (because we called pvm_catchout())
*
*	TC_OUTPUT() {
*		int tid				// of source task
*		int length			// of output
*		char output[length]	// characters printed
*	}
*/

static int
pvmclaimo(mid)
	int mid;
{
	int tid;					/* task */
	int n;						/* length or event code */
	char *p;
	int ptid;
	struct tobuf *op;

/*
	if (TRACE) {
		checktrace(jp, cc, len, cod, src);
		return 0;
	}
*/

	pvm_upkint(&tid, 1, 1);
	pvm_upkint(&n, 1, 1);

	if (!tobuflist) {
		pvmlogprintf("pvmclaimo() message from task t%x\n", tid);
		goto done;
	}

	if (n > 0) {	/* output printed by task */
		if (!(op = tobuf_find(tid))) {
			pvmlogprintf("pvmclaimo() bogus message, no task t%x\n", tid);
			goto done;
		}
		if (n + op->o_len >= op->o_maxl) {
			op->o_maxl = op->o_len + n + 1;
/*
			printf("REALLOC t%x buf to %d\n", tid, op->o_maxl);
*/
			if (op->o_buf)
				op->o_buf = TREALLOC(op->o_buf, op->o_maxl, char);
			else
				op->o_buf = TALLOC(op->o_maxl, char, "");
		}
		pvm_upkbyte(op->o_buf + op->o_len, n, 1);
		op->o_buf[op->o_len + n] = 0;
/*
		printf("UNPACK t%x {%s}\n", tid, op->o_buf + op->o_len);
*/
		p = op->o_buf + op->o_len;
		op->o_len += n;
/* XXX would be more efficient to remove all lines before copying remainder */
		while (p = CINDEX(p, '\n')) {
			*p++ = 0;
			if (outlogff)
				if (pvmshowtaskid)
					fprintf(outlogff, "[t%x] %s\n", tid, op->o_buf);
				else
					fprintf(outlogff, "%s\n", op->o_buf);
			op->o_len -= p - op->o_buf;
			BCOPY(p, op->o_buf, op->o_len + 1);
			p = op->o_buf;
		}

	} else {
		switch (n) {

		case 0:		/* EOF from task */
			if (!(op = tobuf_find(tid))) {
				pvmlogprintf("pvmclaimo() bogus message, no task t%x\n", tid);
				goto done;
			}
			if (op->o_len > 0) {
				if (outlogff)
					if (pvmshowtaskid)
						fprintf(outlogff, "[t%x] %s\n", tid, op->o_buf);
					else
						fprintf(outlogff, "%s\n", op->o_buf);
				op->o_len = 0;
			}
			op->o_flag |= GOTEOF;
			if ((op->o_flag & TASKSTATE) == GOTBOTH) {
				if (outlogff && pvmshowtaskid)
					fprintf(outlogff, "[t%x] EOF\n", tid);
				tobuf_free(op);
			}
			break;

		case -1:	/* spawn creating new task */
			if (!(op = tobuf_find(tid))) {
				op = tobuf_new(tid);
				if (outlogff && pvmshowtaskid)
					fprintf(outlogff, "[t%x] BEGIN\n", tid);
			}
			op->o_flag |= GOTSPAWN;
			if ((op->o_flag & TASKSTATE) == GOTBOTH) {
				if (outlogff && pvmshowtaskid)
					fprintf(outlogff, "[t%x] EOF\n", tid);
				tobuf_free(op);
			}

			break;

		case -2:	/* new task starting up */
			if (!(op = tobuf_find(tid))) {
				op = tobuf_new(tid);
				if (outlogff && pvmshowtaskid)
					fprintf(outlogff, "[t%x] BEGIN\n", tid);
			}
			break;

		default:
			pvmlogprintf("pvmclaimo() bogus message from task t%x\n", tid);
			break;
		}
	}

done:
	pvm_freebuf(mid);
	return 0;
}


/**************************
 **  Exported Functions  **
 **                      **
 **************************/

int
tobuf_dump()
{
	struct tobuf *op;

	if (tobuflist)
		for (op = tobuflist->o_link; op != tobuflist; op = op->o_link) {
			printf("tobuf_dump() t%x\n", op->o_tid);
		}
	else
		printf("tobuf_dump() tobuflist null\n");
	return 0;
}


/************************
 **  Libpvm Functions  **
 **                    **
 ************************/

/*	pvm_catchout()
*
*	Log output of tasks spawned by us to file.
*/

int
pvm_catchout(ff)
	FILE *ff;
{
	if (ff) {
		tobuf_init();
		pvm_setopt(PvmOutputTid, pvm_mytid());
		pvm_setopt(PvmOutputContext, (int)SYSCTX_TC);
		pvm_setopt(PvmOutputCode, (int)TC_OUTPUT);
		outlogff = ff;

	} else {
		pvm_setopt(PvmOutputCode, pvm_getopt(PvmSelfOutputCode));
		pvm_setopt(PvmOutputContext, pvm_getopt(PvmSelfOutputContext));
		pvm_setopt(PvmOutputTid, pvm_getopt(PvmSelfOutputTid));
		outlogff = 0;
	}
	return PvmOk;
}


