/*
* $XConsortium: StripCharP.h,v 1.5 94/04/17 20:12:55 converse Exp $
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

#ifndef _XawStripChartP_h
#define _XawStripChartP_h

#include <X11/Xaw3d/StripChart.h>
#include <X11/Xaw3d/ThreeDP.h>

#define NO_GCS 0
#define FOREGROUND 1 << 0
#define HIGHLIGHT  1 << 1
#define ALL_GCS (FOREGROUND | HIGHLIGHT)

/* New fields for the stripChart widget instance record */

typedef struct {
    Pixel	fgpixel;	/* color index for graph */
    Pixel	hipixel;	/* color index for lines */
    GC	fgGC;		/* graphics context for fgpixel */
    GC	hiGC;		/* graphics context for hipixel */
    
    /* start of graph stuff */
    
    int	update;		/* update frequence */
    int	scale;		/* scale factor */
    int	min_scale;	/* smallest scale factor */
    int	interval;	/* data point interval */
    XPoint * points ;	/* Poly point for repairing graph lines. */
    double max_value;	/* Max Value in window */
    double valuedata[2048];/* record of data points */
    XtIntervalId interval_id;
    XtCallbackList get_value; /* proc to call to fetch load pt */
    int jump_val;		/* Amount to jump on each scroll. */
} StripChartPart;

/* Full instance record declaration */
typedef struct _StripChartRec {
   CorePart core;
   SimplePart simple;
   ThreeDPart threeD;
   StripChartPart strip_chart;
} StripChartRec;

/* New fields for the StripChart widget class record */
typedef struct {int dummy;} StripChartClassPart;

/* Full class record declaration. */
typedef struct _StripChartClassRec {
   CoreClassPart core_class;
   SimpleClassPart simple_class;
   ThreeDClassPart threeD_class;
   StripChartClassPart strip_chart_class;
} StripChartClassRec;

/* Class pointer. */
extern StripChartClassRec stripChartClassRec;

#endif /* _XawStripChartP_h */
