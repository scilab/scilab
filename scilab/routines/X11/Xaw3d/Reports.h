/*
 * $XConsortium: Reports.h,v 1.4 94/04/17 20:12:39 jim Exp $
 *
Copyright (c) 1990  X Consortium

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
 */

#ifndef _Xaw_Reports_h
#define _Xaw_Reports_h

/*
 * XawPannerReport - this structure is used by the reportCallback of the
 * Panner, Porthole, Viewport, and Scrollbar widgets to report its position.
 * All fields must be filled in, although the changed field may be used as
 * a hint as to which fields have been altered since the last report.
 */
typedef struct {
    unsigned int changed;		/* mask, see below */
    Position slider_x, slider_y;	/* location of slider within outer */
    Dimension slider_width, slider_height;  /* size of slider */
    Dimension canvas_width, canvas_height;  /* size of canvas */
} XawPannerReport;

#define XawPRSliderX		(1 << 0)
#define XawPRSliderY		(1 << 1)
#define XawPRSliderWidth	(1 << 2)
#define XawPRSliderHeight	(1 << 3)
#define XawPRCanvasWidth	(1 << 4)
#define XawPRCanvasHeight	(1 << 5)
#define XawPRAll		(63)	/* union of above */

#define XtNreportCallback "reportCallback"
#define XtCReportCallback "reportCallback"

#endif /* _Xaw_Reports_h */
