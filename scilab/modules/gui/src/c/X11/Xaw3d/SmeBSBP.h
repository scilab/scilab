/*
 * $XConsortium: SmeBSBP.h,v 1.8 94/04/17 21:44:11 rws Exp $
 *
Copyright (c) 1989, 1994  X Consortium

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
 * SmeP.h - Private definitions for Sme object
 * 
 */

#ifndef _XawSmeBSBP_h
#define _XawSmeBSBP_h

/***********************************************************************
 *
 * Sme Object Private Data
 *
 ***********************************************************************/

#include <X11/Xaw3d/SmeThreeDP.h>
#include <X11/Xaw3d/SmeBSB.h>

/************************************************************
 *
 * New fields for the Sme Object class record.
 *
 ************************************************************/

typedef struct _SmeBSBClassPart {
  XtPointer extension;
} SmeBSBClassPart;

/* Full class record declaration */
typedef struct _SmeBSBClassRec {
    RectObjClassPart	rect_class;
    SmeClassPart	sme_class;
    SmeThreeDClassPart	sme_threeD_class;
    SmeBSBClassPart	sme_bsb_class;
} SmeBSBClassRec;

extern SmeBSBClassRec smeBSBClassRec;

/* New fields for the Sme Object record */
typedef struct {
    /* resources */
    String label;		/* The entry label. */
    int vert_space;		/* extra vert space to leave, as a percentage
				   of the font height of the label. */
    Pixmap left_bitmap, right_bitmap; /* bitmaps to show. */
    Dimension left_margin, right_margin; /* left and right margins. */
    Pixel foreground;		/* foreground color. */
    XFontStruct * font;		/* The font to show label in. */
    XFontSet fontset;		/* or fontset */
    XtJustify justify;		/* Justification for the label. */

/* private resources. */

    Boolean set_values_area_cleared; /* Remember if we need to unhighlight. */
    GC norm_gc;			/* noral color gc. */
    GC rev_gc;			/* reverse color gc. */
    GC norm_gray_gc;		/* Normal color (grayed out) gc. */
    GC invert_gc;		/* gc for flipping colors. */

    Dimension left_bitmap_width; /* size of each bitmap. */
    Dimension left_bitmap_height;
    Dimension right_bitmap_width;
    Dimension right_bitmap_height;

} SmeBSBPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _SmeBSBRec {
    ObjectPart		object;
    RectObjPart		rectangle;
    SmePart		sme;
    SmeThreeDPart	sme_threeD;
    SmeBSBPart		sme_bsb;
} SmeBSBRec;

/************************************************************
 *
 * Private declarations.
 *
 ************************************************************/

#endif /* _XawSmeBSBP_h */
