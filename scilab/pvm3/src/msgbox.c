
static char rcsid[] =
	"$Id: msgbox.c,v 1.1 2001/04/26 07:47:10 scilab Exp $";

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
 *	msgbox.c
 *
 *	Message mailbox database.
 *
$Log: msgbox.c,v $
Revision 1.1  2001/04/26 07:47:10  scilab
Initial revision

 * Revision 1.17  1997/06/27  17:32:30  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.16  1997/05/07  21:23:54  pvmsrc
 * Fixed manual packing of class name lengths.
 * 	- no longer necessary with (pvm)upkstralloc() usage.
 * Fixed bug:
 * 	- only free pattbuf if !pattglob...  D-Oh!
 *
 * Revision 1.15  1997/05/05  18:17:49  pvmsrc
 * Added new pvmregex.o targets (part of ./regex stuff).
 * 	- extracted mb_names() regex usage to new pvm/regex routines:
 * 		void *pvmcompileregex __ProtoGlarp__ (( char * ));
 * 		int pvmmatchregex __ProtoGlarp__ (( void *, char * ));
 * 		void pvmfreeregex __ProtoGlarp__ (( void ** ));
 *
 * Revision 1.14  1997/05/02  19:44:38  pvmsrc
 * Added GNU Regex Stuff:
 * 	- Makefile targets for regex.o
 * 	- usage in mb_names() / msgbox.c
 *
 * Revision 1.13  1997/05/02  14:54:18  pvmsrc
 * Implemented guts of pvm_getmboxinfo():
 * 	- user library side sends pattern & receives back / allocs array.
 * 	- at pvmd side mb_names() searches class list for matching pattern,
 * 		count up # of entries, and pack up struct info.
 * 		(Note:  does not yet use GNU regex stuff...)
 *
 * Revision 1.12  1997/04/10  17:53:54  pvmsrc
 * Externalized me_new()...
 * 	- for WT_RECVINFO usage...
 *
 * Revision 1.11  1997/04/08  20:06:33  pvmsrc
 * Un-static-ed me_free().
 * 	- so ddpro.c can use it...  D-Oh.
 *
 * Revision 1.10  1997/04/08  19:57:52  pvmsrc
 * Promoted mbox static "classes" to public global "pvmmboxclasses".
 * 	- so pvmd can spank mboxes in ddpro.c...  :-Q
 * 	- renamed everywhere, moved decl / extern to global.[ch].
 *
 * Revision 1.9  1997/04/08  19:41:11  pvmsrc
 * Added me_savetid to pvmmentry struct.
 * 	- for keeping track of original owner on persistent mboxes.
 * 	- allows more careful cleanup in a system reset, set me_tid to 0
 * 		but save original tid in me_savetid first...  :-)
 *
 * Revision 1.8  1997/04/08  18:45:20  pvmsrc
 * Yanked out internal struct defns for mclass & mentry.
 * 	- replaced with msgbox.h header, and pvmmclass / pvmmentry structs.
 *
 * Revision 1.7  1997/04/08  17:53:25  pvmsrc
 * Added new mb_tidy_reset() routine.
 * 	- triggered by task exit during a system reset (when persistent
 * 		mbox entries are owned by task).
 * 	- clear out any persistent mboxes owned by task, or any others
 * 		that have already been cleared.
 *
 * Revision 1.6  1997/04/01  16:39:39  pvmsrc
 * Oops...  PvmMboxOverwritable -> PvmMboxOverWritable...  D-Oh.
 *
 * Revision 1.5  1997/04/01  16:19:25  pvmsrc
 * Modified mbox internals to properly handle new flags syntax.
 * 	- pvm_putinfo():
 * 		* PvmMboxMultiInstance = if index 0 is in use, stick in
 * 			first available higher index.
 * 		* PvmMboxOverwritable = allow anyone to overwrite or delete
 * 			the given mbox entry.
 * 		* PvmMboxPersistent = do not remove mailbox on task exit,
 * 			allow only PVMD to delete on reset (after task exit only).
 * 	- pvm_recvinfo():
 * 		* PvmMboxReadAndDelete = if entry is owned by task or is
 * 			overwritable, read entry and atomically delete, else fail.
 * 		* PvmMboxFirstAvail = if entry not present at given index,
 * 			return next entry by index, else fail.
 *
 * Revision 1.4  1997/01/28  19:26:54  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/24  21:04:48  pvmsrc
 * Moved #include of "global.h" down below other headers:
 * 	- need to have all of the structures / types declared before
 * 		the globals can be declared...
 *
 * Revision 1.2  1996/09/23  23:32:16  pvmsrc
 * Initial Creation - original msgbox.c.
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
#else
#include <strings.h>
#endif
#include <pvm3.h>
#include "pvmalloc.h"
#include "listmac.h"
#include "pmsg.h"
#include "msgbox.h"
#include "global.h"

void *pvmcompileregex __ProtoGlarp__ (( char * ));
int pvmmatchregex __ProtoGlarp__ (( void *, char * ));
void pvmfreeregex __ProtoGlarp__ (( void ** ));


/***************
 **  Globals  **
 **           **
 ***************/

extern int pvmdebmask;					/* from pvmd.c */

/***************
 **  Private  **
 **           **
 ***************/

struct pvmmentry *
me_new(ind)
	int ind;
{
	struct pvmmentry *ep;

	if (ep = TALLOC(1, struct pvmmentry, "pvmmentry")) {
		ep->me_link = ep->me_rlink = ep;
		ep->me_tid = 0;
		ep->me_savetid = 0;
		ep->me_flags = PvmMboxDefault;
		ep->me_msg = 0;
		ep->me_ind = ind;
	}
	return ep;
}


static struct pvmmclass *
mc_new(name)
	char *name;
{
	struct pvmmclass *np;

	if (np = TALLOC(1, struct pvmmclass, "pvmmclass")) {
		if (name) {
			np->mc_name = STRALLOC(name);
			np->mc_ent = me_new(-1);
			LISTPUTBEFORE(pvmmboxclasses, np, mc_link, mc_rlink);

		} else {
			np->mc_link = np->mc_rlink = np;
			np->mc_name = 0;
			np->mc_ent = 0;
		}
	}
	return np;
}


int
me_free(np, ep)
	struct pvmmclass *np;
	struct pvmmentry *ep;
{
	LISTDELETE(ep, me_link, me_rlink);
	pmsg_unref(ep->me_msg);
	PVM_FREE(ep);
	if (np->mc_ent->me_link == np->mc_ent) {
		LISTDELETE(np, mc_link, mc_rlink);
		PVM_FREE(np->mc_name);
		PVM_FREE(np->mc_ent);
		PVM_FREE(np);
	}
	return 0;
}


static struct pvmmclass *
mc_find(name)
	char *name;
{
	struct pvmmclass *np;

	for (np = pvmmboxclasses->mc_link; np != pvmmboxclasses;
			np = np->mc_link)
		if (!strcmp(np->mc_name, name))
			return np;
	return (struct pvmmclass *)0;
}


int
mb_init()
{
	pvmmboxclasses = mc_new((char*)0);
	return 0;
}


int
mb_insert(tid, name, req, flags, mp)
	int tid;				/* owner task */
	char *name;				/* class name */
	int req;				/* index requested or -1 for lowest avail */
	int flags;
	struct pmsg *mp;		/* message to store */
{
	struct pvmmclass *np;
	struct pvmmentry *ep;
	struct pvmmentry *ep2 = 0;

	if (!(np = mc_find(name)))
		if (!(np = mc_new(name)))
			return PvmNoMem;

	for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep->me_link)
		if (ep->me_ind >= req)
			break;

	/* default insert is "locked"... */

	if (flags & PvmMboxMultiInstance) {
		for (; ep != np->mc_ent; ep = ep->me_link) {
			if (ep->me_ind != req)
				break;
			if ( (ep->me_flags & PvmMboxOverWritable)
					|| tid == ep->me_tid ) {
				ep2 = ep;
				break;
			}
			req = ep->me_ind + 1;
		}
	} else {
		if (ep->me_ind == req) {
			if ( !(ep->me_flags & PvmMboxOverWritable)
					&& tid != ep->me_tid) {
				return PvmDenied;
			}
			ep2 = ep;
		}
	}

	if (ep2) {
		ep = ep2->me_link;
		LISTDELETE(ep2, me_link, me_rlink);
		pmsg_unref(ep2->me_msg);
		PVM_FREE(ep2);
	}
	ep2 = me_new(req);
	ep2->me_tid = tid;
	ep2->me_flags = flags;
	ep2->me_msg = mp;
	LISTPUTBEFORE(ep, ep2, me_link, me_rlink);

	return req;
}


int
mb_delete(tid, name, req, flags)
	int tid;				/* owner task */
	char *name;				/* class name */
	int req;				/* index */
	int flags;				/* options */
{
	struct pvmmclass *np;
	struct pvmmentry *ep = 0;

	if (np = mc_find(name)) {
		for (ep = np->mc_ent->me_link; ep != np->mc_ent;
				ep = ep->me_link) {
			if (ep->me_ind == req)
				break;
		}
		if (ep == np->mc_ent)
			ep = 0;
	}

	if (!ep)
		return PvmNotFound;

	if ( !(ep->me_flags & PvmMboxOverWritable) && tid != ep->me_tid )
		return PvmDenied;

	req = ep->me_ind;
	me_free(np, ep);

	return req;
}


int
mb_lookup(tid, name, req, flags, mpp)
	int tid;				/* owner task */
	char *name;				/* class name */
	int req;				/* index requested or -1 for first avail */
	int flags;
	struct pmsg **mpp;		/* message return */
{
	struct pvmmclass *np;
	struct pvmmentry *ep = 0;

	if (np = mc_find(name)) {
		for (ep = np->mc_ent->me_link; ep != np->mc_ent;
				ep = ep->me_link) {
			if (ep->me_ind >= req)
				break;
		}
		if (ep == np->mc_ent)
			ep = 0;
		else if (!(flags & PvmMboxFirstAvail) && ep->me_ind != req)
			ep = 0;
	}

	if (ep) {
		if ( flags & PvmMboxReadAndDelete ) {
			if ( (ep->me_flags & PvmMboxOverWritable)
					|| tid == ep->me_tid ) {
				req = ep->me_ind;
				ep->me_msg->m_ref++;
				*mpp = ep->me_msg;
				me_free(np, ep);
			} else {
				req = PvmDenied;
				*mpp = (struct pmsg *) NULL;
			}
		} else {
			req = ep->me_ind;
			ep->me_msg->m_ref++;
			*mpp = ep->me_msg;
		}
	} else
		req = PvmNotFound;

	return req;
}


int
mb_names(tid, pattern, mp)
	int tid;
	char *pattern;
	struct pmsg *mp;
{
	void *pattbuff;

	struct pvmmclass *np;
	struct pvmmentry *ep;

	int pattglob = 0;
	int cnt;

	/* Check for "*" Global Match Pattern */

	if ( !strcmp( pattern, "*" ) )
		pattglob = 1;

	/* Compile Regular Expression */

	if ( !pattglob )
		pattbuff = pvmcompileregex( pattern );

	/* Count # of Classes */

	cnt = 0;

	for ( np = pvmmboxclasses->mc_link; np != pvmmboxclasses;
			np = np->mc_link )
	{
		if ( pattglob
				|| ( ( pattbuff ) ?
					( pvmmatchregex( pattbuff, np->mc_name ) )
					: ( !strcmp( pattern, np->mc_name ) ) ) )
		{
			cnt++;
		}
	}

	pkint( mp, cnt );

	/* Pack Class Info */

	for ( np = pvmmboxclasses->mc_link; np != pvmmboxclasses;
			np = np->mc_link )
	{
		if ( pattglob
				|| ( ( pattbuff ) ?
					( pvmmatchregex( pattbuff, np->mc_name ) )
					: ( !strcmp( pattern, np->mc_name ) ) ) )
		{
			pkstr( mp, np->mc_name );

			/* Count # of Entries */

			cnt = 0;

			for ( ep = np->mc_ent->me_link; ep != np->mc_ent;
					ep = ep->me_link )
				cnt++;

			pkint( mp, cnt );

			/* Pack Entry Info */

			for ( ep = np->mc_ent->me_link; ep != np->mc_ent;
					ep = ep->me_link )
			{
				pkint( mp, ep->me_ind );
				pkint( mp, ep->me_tid );
				pkint( mp, ep->me_flags );
			}
		}
	}

	if ( !pattglob && pattbuff )
		pvmfreeregex( &pattbuff );

	return 0;
}


int
mb_dump(np)
	struct pvmmclass *np;
{
	struct pvmmentry *ep;

	pvmlogprintf("<%s>\n", np->mc_name);
	for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep->me_link) {
		pvmlogprintf("%d (0x%x): owner 0x%x (was 0x%x) flags %d\n",
				ep->me_ind, ep->me_ind, ep->me_tid, ep->me_savetid,
				ep->me_flags);
		pmsg_dump(ep->me_msg, 2);
	}
	return 0;
}


int
mb_dumpall()
{
	struct pvmmclass *np;

	pvmlogerror("mb_dumpall():\n");
	for (np = pvmmboxclasses->mc_link; np != pvmmboxclasses;
			np = np->mc_link)
		mb_dump(np);
	return 0;
}


/*	mb_tidy()
*
*	Task has exited.
*	Remove any non-sticky entries owned by it.
*	Remove its tid as owner for sticky entries and reset Lock.
*/

int
mb_tidy(tid)
	int tid;
{
	struct pvmmclass *np, *np2;
	struct pvmmentry *ep, *ep2;

	if (!tid)
		return 0;

	if (pvmdebmask & PDMMBOX)
		pvmlogprintf("mb_tidy() tid 0x%x\n", tid);

	for (np = pvmmboxclasses->mc_link; np != pvmmboxclasses; np = np2) {
		np2 = np->mc_link;
		for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep2) {
			ep2 = ep->me_link;
			if (ep->me_tid == tid) {
				if (ep->me_flags & PvmMboxPersistent) {
					ep->me_savetid = ep->me_tid;
					ep->me_tid = 0;
				} else {
					me_free(np, ep);
					if (np2->mc_rlink != np)
						break;
				}
			}
		}
	}
	return 0;
}


/*	mb_tidy_reset()
*
*	Task has exited during a system reset.
*	Remove any persistent entries owned by it, and any other
*	persistent entries that have already been cleared.
*/

int
mb_tidy_reset(tid)
	int tid;
{
	struct pvmmclass *np, *np2;
	struct pvmmentry *ep, *ep2;

	if (!tid)
		return 0;

	if (pvmdebmask & PDMMBOX)
		pvmlogprintf("mb_tidy_reset() tid 0x%x\n", tid);

	for (np = pvmmboxclasses->mc_link; np != pvmmboxclasses; np = np2) {
		np2 = np->mc_link;
		for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep2) {
			ep2 = ep->me_link;
			/* mb_tidy() & mb_tidy_reset() could be in any order... */
			if ( (ep->me_flags & PvmMboxPersistent)
					&& ( ep->me_tid == tid
						|| (!(ep->me_tid) && ep->me_savetid == tid) ) )
			{
				me_free(np, ep);
				if (np2->mc_rlink != np)
					break;
			}
		}
	}
	return 0;
}

