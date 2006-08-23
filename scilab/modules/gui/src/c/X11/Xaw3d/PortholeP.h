/*
 * $XConsortium: PortholeP.h,v 1.3 94/04/17 20:12:36 kaleb Exp $
 *
Copyright (c) 1990, 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#ifndef _XawPortholeP_h
#define _XawPortholeP_h

#include <X11/Xaw3d/Porthole.h>

typedef struct {			/* new fields in widget class */
    int dummy;
} PortholeClassPart;

typedef struct _PortholeClassRec {	/* Porthole widget class */
    CoreClassPart core_class;
    CompositeClassPart composite_class;
    PortholeClassPart porthole_class;
} PortholeClassRec;


typedef struct {			/* new fields in widget */
    /* resources... */
    XtCallbackList report_callbacks;	/* callback/Callback */
    /* private data... */
} PortholePart;

typedef struct _PortholeRec {
    CorePart core;
    CompositePart composite;
    PortholePart porthole;
} PortholeRec;


/*
 * external declarations
 */
extern PortholeClassRec portholeClassRec;


#endif /* _XawPortholeP_h */
