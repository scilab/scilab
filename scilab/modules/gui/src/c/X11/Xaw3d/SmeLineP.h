/*
 * $XConsortium: SmeLineP.h,v 1.4 94/04/17 20:12:54 kit Exp $
 *
Copyright (c) 1989  X Consortium

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
 * Author:  Chris D. Peterson, MIT X Consortium
 */

/* 
 * SmeLineP.h - Private definitions for SmeLine widget
 * 
 */

#ifndef _XawSmeLineP_h
#define _XawSmeLineP_h

/***********************************************************************
 *
 * SmeLine Widget Private Data
 *
 ***********************************************************************/

#include <X11/Xaw3d/SmeP.h>
#include <X11/Xaw3d/SmeLine.h>

/************************************************************
 *
 * New fields for the SmeLine widget class record.
 *
 ************************************************************/

typedef struct _SmeLineClassPart {
  XtPointer extension;
} SmeLineClassPart;

/* Full class record declaration */
typedef struct _SmeLineClassRec {
    RectObjClassPart    rect_class;
    SmeClassPart	sme_class;
    SmeLineClassPart	sme_line_class;
} SmeLineClassRec;

extern SmeLineClassRec smeLineClassRec;

/* New fields for the SmeLine widget record */
typedef struct {
    /* resources */
    Pixel foreground;		/* Foreground color. */
    Pixmap stipple;		/* Line Stipple. */
    Dimension line_width;	/* Width of the line. */

    /* private data.  */

    GC gc;			/* Graphics context for drawing line. */
} SmeLinePart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _SmeLineRec {
  ObjectPart     object;
  RectObjPart    rectangle;
  SmePart	 sme;
  SmeLinePart	 sme_line;
} SmeLineRec;

/************************************************************
 *
 * Private declarations.
 *
 ************************************************************/

#endif /* _XawSmeLineP_h */
