/*
 * $XConsortium: CvtCache.c,v 1.6 90/12/19 18:21:33 converse Exp $
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
 * 
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xmu/CvtCache.h>

extern char *malloc();

static XmuDisplayQueue *dq = NULL;
static int _CloseDisplay(), _FreeCCDQ();



/*
 * internal utility callbacks
 */

static int _FreeCCDQ (q)
    XmuDisplayQueue *q;
{
    XmuDQDestroy (dq, False);
    dq = NULL;
}


static int _CloseDisplay (q, e)
    XmuDisplayQueue *q;
    XmuDisplayQueueEntry *e;
{
    XmuCvtCache *c;
    extern void _XmuStringToBitmapFreeCache();

    if (e && (c = (XmuCvtCache *)(e->data))) {
	_XmuStringToBitmapFreeCache (c);
	/* insert calls to free any cached memory */

    }
    return 0;
}

static void _InitializeCvtCache (c)
    register XmuCvtCache *c;
{
    extern void _XmuStringToBitmapInitCache();

    _XmuStringToBitmapInitCache (c);
    /* insert calls to init any cached memory */
}


/*
 * XmuCCLookupDisplay - return the cache entry for the indicated display;
 * initialize the cache if necessary
 */
XmuCvtCache *_XmuCCLookupDisplay (dpy)
    Display *dpy;
{
    XmuDisplayQueueEntry *e;

    /*
     * If no displays have been added before this, create the display queue.
     */
    if (!dq) {
	dq = XmuDQCreate (_CloseDisplay, _FreeCCDQ, NULL);
	if (!dq) return NULL;
    }
    
    /*
     * See if the display is already there
     */
    e = XmuDQLookupDisplay (dq, dpy);	/* see if it's there */
    if (!e) {				/* else create it */
	XmuCvtCache *c = (XmuCvtCache *) malloc (sizeof (XmuCvtCache));
	if (!c) return NULL;

	/*
	 * Add the display to the queue
	 */
	e = XmuDQAddDisplay (dq, dpy, (caddr_t) c);
	if (!e) {
	    free ((char *) c);
	    return NULL;
	}

	/*
	 * initialize fields in cache
	 */
	_InitializeCvtCache (c);
    }

    /*
     * got it
     */
    return (XmuCvtCache *)(e->data);
}


