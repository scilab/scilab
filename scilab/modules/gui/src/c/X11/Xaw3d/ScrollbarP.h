/* MODIFIED ATHENA SCROLLBAR (USING ARROWHEADS AT ENDS OF TRAVEL) */
/* Modifications Copyright 1992 by Mitch Trachtenberg             */
/* Rights, permissions, and disclaimer of warranty are as in the  */
/* DEC and MIT notice below.  See usage warning in .c file.       */

/*
 * $XConsortium: ScrollbarP.h,v 1.2 90/04/11 17:33:53 jim Exp $
 */

/***********************************************************

Copyright (c) 1987, 1988  X Consortium

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


Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef _ScrollbarP_h
#define _ScrollbarP_h

#include <X11/Xaw3d/Scrollbar.h>
#include <X11/Xaw3d/ThreeDP.h>

typedef struct {
     /* public */
    Pixel	  foreground;	/* thumb foreground color */
    XtOrientation orientation;	/* horizontal or vertical */
    XtCallbackList scrollProc;	/* proportional scroll */
    XtCallbackList thumbProc;	/* jump (to position) scroll */
    XtCallbackList jumpProc;	/* same as thumbProc but pass data by ref */
    Pixmap	  thumb;	/* thumb color */
#ifndef ARROW_SCROLLBAR
    Cursor        upCursor;	/* scroll up cursor */
    Cursor        downCursor;	/* scroll down cursor */
    Cursor        leftCursor;	/* scroll left cursor */
    Cursor        rightCursor;	/* scroll right cursor */
    Cursor        verCursor;	/* scroll vertical cursor */
    Cursor        horCursor;	/* scroll horizontal cursor */
#endif
    float	  top;		/* What percent is above the win's top */
    float	  shown;	/* What percent is shown in the win */
    Dimension	  length;	/* either height or width */
    Dimension	  thickness;	/* either width or height */
    Dimension	  min_thumb;	/* minium size for the thumb. */
    float         picked;       /* How much of the thumb is picked *
				 * when scrolling starts */

     /* private */
#ifdef ARROW_SCROLLBAR
    XtIntervalId  timer_id;     /* autorepeat timer; remove on destruction */
    char	  scroll_mode;	/* 0:none 1:up/back 2:track 3:down/forward */
#else
    Cursor        inactiveCursor; /* the normal cursor for scrollbar */
    char          direction;	/* a scroll has started; which direction */
#endif
    GC		  gc;		/* a (shared) gc */
    Position	  topLoc;	/* Pixel that corresponds to top */
    Dimension	  shownLength;	/* Num pixels corresponding to shown */
    Boolean       pick_top;     /* pick thumb at top or anywhere*/

} ScrollbarPart;

typedef struct _ScrollbarRec {
    CorePart		core;
    SimplePart		simple;
    ThreeDPart		threeD;
    ScrollbarPart	scrollbar;
} ScrollbarRec;

typedef struct {int empty;} ScrollbarClassPart;

typedef struct _ScrollbarClassRec {
    CoreClassPart		core_class;
    SimpleClassPart		simple_class;
    ThreeDClassPart		threeD_class;
    ScrollbarClassPart		scrollbar_class;
} ScrollbarClassRec;

extern ScrollbarClassRec scrollbarClassRec;

#endif /* _ScrollbarP_h */
