/*
* $XConsortium: TextSinkP.h,v 1.6 94/04/17 20:13:13 kaleb Exp $
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
 * TextSinkP.h - Private definitions for TextSink object
 * 
 */

#ifndef _XawTextSinkP_h
#define _XawTextSinkP_h

/***********************************************************************
 *
 * TextSink Object Private Data
 *
 ***********************************************************************/

#include <X11/Xaw3d/TextSink.h>
#include <X11/Xaw3d/TextP.h>	/* This source works with the Text widget. */
#include <X11/Xaw3d/TextSrcP.h>	/* This source works with the Text Source. */

/************************************************************
 *
 * New fields for the TextSink object class record.
 *
 ************************************************************/

typedef struct _TextSinkClassPart {
  void (*DisplayText)();
  void (*InsertCursor)();
  void (*ClearToBackground)();
  void (*FindPosition)();
  void (*FindDistance)();
  void (*Resolve)();
  int  (*MaxLines)();
  int  (*MaxHeight)();
  void (*SetTabs)();		
  void (*GetCursorBounds)();	
} TextSinkClassPart;

/* Full class record declaration */
typedef struct _TextSinkClassRec {
    ObjectClassPart     object_class;
    TextSinkClassPart	text_sink_class;
} TextSinkClassRec;

extern TextSinkClassRec textSinkClassRec;

/* New fields for the TextSink object record */
typedef struct {
    /* resources */
    Pixel foreground;		/* Foreground color. */
    Pixel background;		/* Background color. */

    /* private state. */
    Position *tabs;		/* The tab stops as pixel values. */
    short    *char_tabs;	/* The tabs stops as character values. */
    int      tab_count;		/* number of items in tabs */

} TextSinkPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _TextSinkRec {
  ObjectPart    object;
  TextSinkPart	text_sink;
} TextSinkRec;

/************************************************************
 *
 * Private declarations.
 *
 ************************************************************/

typedef int (*_XawSinkIntFunc)();
typedef void (*_XawSinkVoidFunc)();

#define XtInheritDisplayText          ((_XawSinkVoidFunc) _XtInherit)
#define XtInheritInsertCursor         ((_XawSinkVoidFunc) _XtInherit)
#define XtInheritClearToBackground    ((_XawSinkVoidFunc) _XtInherit)
#define XtInheritFindPosition         ((_XawSinkVoidFunc) _XtInherit)
#define XtInheritFindDistance         ((_XawSinkVoidFunc) _XtInherit)
#define XtInheritResolve              ((_XawSinkVoidFunc) _XtInherit)
#define XtInheritMaxLines             ((_XawSinkIntFunc) _XtInherit)
#define XtInheritMaxHeight            ((_XawSinkIntFunc) _XtInherit)
#define XtInheritSetTabs              ((_XawSinkVoidFunc) _XtInherit)
#define XtInheritGetCursorBounds      ((_XawSinkVoidFunc) _XtInherit)

#endif /* _XawTextSinkP_h */
