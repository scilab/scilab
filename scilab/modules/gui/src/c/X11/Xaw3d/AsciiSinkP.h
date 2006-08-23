/*
* $XConsortium: AsciiSinkP.h,v 1.4 94/04/17 20:11:44 kaleb Exp $
*/


/***********************************************************

Copyright (c) 1987, 1988, 1994  X Consortium

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

/* 
 * asciiSinkP.h - Private definitions for asciiSink object
 * 
 */

#ifndef _XawAsciiSinkP_h
#define _XawAsciiSinkP_h

/***********************************************************************
 *
 * AsciiSink Object Private Data
 *
 ***********************************************************************/

#include <X11/Xaw3d/TextSinkP.h> 
#include <X11/Xaw3d/AsciiSink.h> 

/************************************************************
 *
 * New fields for the AsciiSink object class record.
 *
 ************************************************************/

typedef struct _AsciiSinkClassPart {
  int foo;
} AsciiSinkClassPart;

/* Full class record declaration */

typedef struct _AsciiSinkClassRec {
    ObjectClassPart     object_class;
    TextSinkClassPart	text_sink_class;
    AsciiSinkClassPart	ascii_sink_class;
} AsciiSinkClassRec;

extern AsciiSinkClassRec asciiSinkClassRec;

/* New fields for the AsciiSink object record */
typedef struct {
    /* public resources */
    XFontStruct *font;          /* Font to draw in. */
    Boolean echo;
    Boolean display_nonprinting;

    /* private state */
    GC normgc, invgc, xorgc;
    Pixmap insertCursorOn;
    XawTextInsertState laststate;
    short cursor_x, cursor_y;	/* Cursor Location. */
} AsciiSinkPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _AsciiSinkRec {
    ObjectPart          object;
    TextSinkPart	text_sink;
    AsciiSinkPart	ascii_sink;
} AsciiSinkRec;

#endif /* _XawAsciiSinkP_h */

