/*
 * $XConsortium: UpdMapHint.c,v 1.1 89/09/22 12:07:37 jim Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
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
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Bool XmuUpdateMapHints (dpy, w, hints)
    Display *dpy;
    Window w;
    XSizeHints *hints;
{
    static XSizeHints *shp = NULL;

    if (!hints) {				/* get them first */
	long supp;

	if (!shp) {
	    shp = XAllocSizeHints();
	    if (!shp) return False;
	}
	if (!XGetWMNormalHints (dpy, w, shp, &supp)) return False;
	hints = shp;
    }
    hints->flags &= ~(PPosition|PSize);
    hints->flags |= (USPosition|USSize);
    XSetWMNormalHints (dpy, w, hints);
    return True;
}
    
