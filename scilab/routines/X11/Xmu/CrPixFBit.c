/*
 * $XConsortium: CrPixFBit.c,v 1.3 90/12/19 18:58:49 converse Exp $
 *
 * Copyright 1988 Massachusetts Institute of Technology
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
 * This file contains miscellaneous utility routines and is not part of the
 * Xlib standard.
 *
 * Public entry points:
 *
 *     XmuCreatePixmapFromBitmap	make a pixmap from a bitmap
 */

#include <X11/Xos.h>
#include <X11/Xlib.h>

Pixmap XmuCreatePixmapFromBitmap (dpy, d, bitmap, width, height, depth,
				  fore, back)
    Display *dpy;			/* connection to X server */
    Drawable d;				/* drawable indicating screen */
    Pixmap bitmap;			/* single plane pixmap */
    unsigned int width, height;		/* dimensions of bitmap and pixmap */
    unsigned int depth;			/* depth of pixmap to create */
    unsigned long fore, back;		/* colors to use */
{
    Pixmap pixmap;

    pixmap = XCreatePixmap (dpy, d, width, height, depth);
    if (pixmap != None) {
	GC gc;
	XGCValues xgcv;

	xgcv.foreground = fore;
	xgcv.background = back;
	xgcv.graphics_exposures = False;

	gc = XCreateGC (dpy, d,
			(GCForeground | GCBackground | GCGraphicsExposures),
			&xgcv);
	if (gc) {
	    XCopyPlane (dpy, bitmap, pixmap, gc, 0, 0, width, height, 0, 0, 1);
	    XFreeGC (dpy, gc);
	} else {
	    XFreePixmap (dpy, pixmap);
	    pixmap = None;
	}
    }
    return pixmap;
}
