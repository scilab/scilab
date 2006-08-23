/* $XConsortium: MultiSinkP.h,v 1.2 94/04/17 20:12:24 kaleb Exp $ */

/*
 * Copyright 1991 by OMRON Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name OMRON not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  OMRON make no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * OMRON DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OMRON BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *      Author: Li Yuhong	 OMRON Corporation
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
 * multiSinkP.h - Private definitions for multiSink object
 * 
 */

#ifndef _XawMultiSinkP_h
#define _XawMultiSinkP_h

/***********************************************************************
 *
 * MultiSink Object Private Data
 *
 ***********************************************************************/

#include <X11/Xaw3d/TextSinkP.h> 
#include <X11/Xaw3d/MultiSink.h> 

/************************************************************
 *
 * New fields for the MultiSink object class record.
 *
 ************************************************************/

typedef struct _MultiSinkClassPart {
  int foo;
} MultiSinkClassPart;

/* Full class record declaration */

typedef struct _MultiSinkClassRec {
    ObjectClassPart     object_class;
    TextSinkClassPart	text_sink_class;
    MultiSinkClassPart	multi_sink_class;
} MultiSinkClassRec;

extern MultiSinkClassRec multiSinkClassRec;

/* New fields for the MultiSink object record */
typedef struct {
    /* public resources */
    Boolean echo;
    Boolean display_nonprinting;

    /* private state */
    GC normgc, invgc, xorgc;
    Pixmap insertCursorOn;
    XawTextInsertState laststate;
    short cursor_x, cursor_y;	/* Cursor Location. */
    XFontSet fontset; /* font set to draw */
} MultiSinkPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _MultiSinkRec {
    ObjectPart          object;
    TextSinkPart	text_sink;
    MultiSinkPart	multi_sink;
} MultiSinkRec;


/********************************************
 *
 * Semi-private functions
 * for use by other Xaw modules only
 *
 *******************************************/

extern void _XawMultiSinkPosToXY(
#if NeedFunctionPrototypes
    Widget			/* w */,
    XawTextPosition		/* pos */,
    Position *			/* x */,
    Position *			/*y */
#endif
);

#endif /* _XawMultiSinkP_h */

