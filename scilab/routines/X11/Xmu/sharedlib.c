/*
 * $XConsortium: sharedlib.c,v 1.6 91/07/31 21:12:30 keith Exp $
 * 
 * Copyright 1991 Massachusetts Institute of Technology
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
 */

#if defined(SUNSHLIB) && !defined(SHAREDCODE)

#include "Atoms.h"

struct _AtomRec {
    char *name;
    struct _DisplayRec* head;
};

#if __STDC__ && !defined(UNIXCPP)
#define DeclareAtom(atom) \
extern struct _AtomRec __##atom; \
AtomPtr _##atom = &__##atom;
#else
#define DeclareAtom(atom) \
extern struct _AtomRec __/**/atom; \
AtomPtr _/**/atom = &__/**/atom;
#endif

DeclareAtom(XA_ATOM_PAIR)
DeclareAtom(XA_CHARACTER_POSITION)
DeclareAtom(XA_CLASS)
DeclareAtom(XA_CLIENT_WINDOW)
DeclareAtom(XA_CLIPBOARD)
DeclareAtom(XA_COMPOUND_TEXT)
DeclareAtom(XA_DECNET_ADDRESS)
DeclareAtom(XA_DELETE)
DeclareAtom(XA_FILENAME)
DeclareAtom(XA_HOSTNAME)
DeclareAtom(XA_IP_ADDRESS)
DeclareAtom(XA_LENGTH)
DeclareAtom(XA_LIST_LENGTH)
DeclareAtom(XA_NAME)
DeclareAtom(XA_NET_ADDRESS)
DeclareAtom(XA_NULL)
DeclareAtom(XA_OWNER_OS)
DeclareAtom(XA_SPAN)
DeclareAtom(XA_TARGETS)
DeclareAtom(XA_TEXT)
DeclareAtom(XA_TIMESTAMP)
DeclareAtom(XA_USER)

#endif /* SUNSHLIB */
