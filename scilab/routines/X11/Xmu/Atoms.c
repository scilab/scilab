/* $XConsortium: Atoms.c,v 1.15 91/06/30 17:49:57 rws Exp $
 *
 * Copyright 1988 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

/*
 * This file contains routines to cache atoms, avoiding multiple
 * server round-trips.  Not so useful now that Xlib caches them.
 *
 * Public entry points:
 *
 *	XmuMakeAtom		creates & initializes an opaque AtomRec
 *	XmuInternAtom		fetches an Atom from cache or Display
 *	XmuInternStrings	fetches multiple Atoms as strings
 *	XmuGetAtomName		returns name of an Atom
 *	XmuNameOfAtom		returns name from an AtomPtr
 */

#include <X11/Intrinsic.h>
#include "Atoms.h"

typedef struct _DisplayRec {
    struct _DisplayRec* next;
    Display *dpy;
    Atom atom;
} DisplayRec;

struct _AtomRec {
    char *name;
    DisplayRec* head;
};

#ifdef SUNSHLIB
#define STATIC
#else
#define STATIC static
#endif

#if __STDC__ && !defined(UNIXCPP)
#define DeclareAtom(atom,text) \
STATIC struct _AtomRec __##atom = { text, NULL }; \
AtomPtr _##atom = &__##atom;
#else
#define DeclareAtom(atom,text) \
STATIC struct _AtomRec __/**/atom = { text, NULL }; \
AtomPtr _/**/atom = &__/**/atom;
#endif

DeclareAtom(XA_ATOM_PAIR,		"ATOM_PAIR"		)
DeclareAtom(XA_CHARACTER_POSITION,	"CHARACTER_POSITION"	)
DeclareAtom(XA_CLASS,			"CLASS"			)
DeclareAtom(XA_CLIENT_WINDOW,		"CLIENT_WINDOW"		)
DeclareAtom(XA_CLIPBOARD,		"CLIPBOARD"		)
DeclareAtom(XA_COMPOUND_TEXT,		"COMPOUND_TEXT"		)
DeclareAtom(XA_DECNET_ADDRESS,		"DECNET_ADDRESS"	)
DeclareAtom(XA_DELETE,			"DELETE"		)
DeclareAtom(XA_FILENAME,		"FILENAME"		)
DeclareAtom(XA_HOSTNAME,		"HOSTNAME"		)
DeclareAtom(XA_IP_ADDRESS,		"IP_ADDRESS"		)
DeclareAtom(XA_LENGTH,			"LENGTH"		)
DeclareAtom(XA_LIST_LENGTH,		"LIST_LENGTH"		)
DeclareAtom(XA_NAME,			"NAME"			)
DeclareAtom(XA_NET_ADDRESS,		"NET_ADDRESS"		)
DeclareAtom(XA_NULL,			"NULL"			)
DeclareAtom(XA_OWNER_OS,		"OWNER_OS"		)
DeclareAtom(XA_SPAN,			"SPAN"			)
DeclareAtom(XA_TARGETS,			"TARGETS"		)
DeclareAtom(XA_TEXT,			"TEXT"			)
DeclareAtom(XA_TIMESTAMP,		"TIMESTAMP"		)
DeclareAtom(XA_USER,			"USER"			)

/******************************************************************

  Public procedures

 ******************************************************************/


#if NeedFunctionPrototypes
AtomPtr XmuMakeAtom(_Xconst char *name)
#else
AtomPtr XmuMakeAtom(name)
    char* name;
#endif
{
    AtomPtr ptr = XtNew(struct _AtomRec);
    ptr->name = (char *) name;
    ptr->head = NULL;
    return ptr;
}

char* XmuNameOfAtom(atom_ptr)
    AtomPtr atom_ptr;
{
    return atom_ptr->name;
}


Atom XmuInternAtom(d, atom_ptr)
    Display *d;
    AtomPtr atom_ptr;
{
    DisplayRec* display_rec;
    for (display_rec = atom_ptr->head; display_rec != NULL;
	 display_rec = display_rec->next) {
	if (display_rec->dpy == d)
	    return display_rec->atom;
    }
    display_rec = XtNew(DisplayRec);
    display_rec->next = atom_ptr->head;
    atom_ptr->head = display_rec;
    display_rec->dpy = d;
    display_rec->atom = XInternAtom(d, atom_ptr->name, False);
    return display_rec->atom;
}


char *XmuGetAtomName(d, atom)
    Display *d;
    Atom atom;
{
    if (atom == 0) return "(BadAtom)";
    return XGetAtomName(d, atom);
}

/* convert (names, count) to a list of atoms. Caller allocates list */
void XmuInternStrings(d, names, count, atoms)
    Display *d;
    register String *names;
    register Cardinal count;
    register Atom *atoms;		/* return */
{
    register int i;

    for (i = 0; i < count; i++) {
	atoms[i] = XInternAtom(d, names[i], False);
    }
    return;
}
