/*
 * $XConsortium: StrToGrav.c,v 1.4 90/11/30 17:00:50 rws Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
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
 */


#include <X11/Intrinsic.h>
#include <X11/Xmu/Converters.h>

#define done(address, type) \
        { (*toVal).size = sizeof(type); (*toVal).addr = (caddr_t) address; }

static struct _namepair {
    XrmQuark quark;
    char *name;
    XtGravity gravity;
} names[] = {
    { NULLQUARK, XtEForget, ForgetGravity },
    { NULLQUARK, XtENorthWest, NorthWestGravity },
    { NULLQUARK, XtENorth, NorthGravity },
    { NULLQUARK, XtENorthEast, NorthEastGravity },
    { NULLQUARK, XtEWest, WestGravity },
    { NULLQUARK, XtECenter, CenterGravity },
    { NULLQUARK, XtEEast, EastGravity },
    { NULLQUARK, XtESouthWest, SouthWestGravity },
    { NULLQUARK, XtESouth, SouthGravity },
    { NULLQUARK, XtESouthEast, SouthEastGravity },
    { NULLQUARK, XtEStatic, StaticGravity },
    { NULLQUARK, XtEUnmap, UnmapGravity },
    { NULLQUARK, XtEleft, WestGravity },
    { NULLQUARK, XtEtop, NorthGravity },
    { NULLQUARK, XtEright, EastGravity },
    { NULLQUARK, XtEbottom, SouthGravity },
    { NULLQUARK, NULL, ForgetGravity }
};

void XmuCvtStringToGravity (args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
{
    static Boolean haveQuarks = FALSE;
    char lowerName[40];
    XrmQuark q;
    char *s;
    struct _namepair *np;

    if (*num_args != 0)
        XtWarningMsg("wrongParameters","cvtStringToGravity","XtToolkitError",
                  "String to Gravity conversion needs no extra arguments",
                  (String *) NULL, (Cardinal *)NULL);

    if (!haveQuarks) {
	for (np = names; np->name; np++) {
	    np->quark = XrmPermStringToQuark (np->name);
	}
	haveQuarks = TRUE;
    }

    s = (char *) fromVal->addr;
    if (strlen(s) < sizeof lowerName) {
	XmuCopyISOLatin1Lowered (lowerName, s);
	q = XrmStringToQuark (lowerName);

	for (np = names; np->name; np++) {
	    if (np->quark == q) {
		done (&np->gravity, XtGravity);
		return;
	    }
	}
    }
    XtStringConversionWarning((char *) fromVal->addr, XtRGravity);
}
