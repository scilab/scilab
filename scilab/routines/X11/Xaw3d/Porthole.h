/*
 * $XConsortium: Porthole.h,v 1.2 94/04/17 20:12:35 jim Exp $
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
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#ifndef _XawPorthole_h
#define _XawPorthole_h

#include <X11/Xaw3d/Reports.h>

/*****************************************************************************
 * 
 * Porthole Widget (subclass of Composite)
 * 
 * This widget is similar to a viewport without scrollbars.  Child movement
 * is done by external panners or scrollbars.
 * 
 * Parameters:
 * 
 *  Name		Class		Type		Default
 *  ----		-----		----		-------
 * 
 *  background		Background	Pixel		XtDefaultBackground
 *  border	        BorderColor	Pixel		XtDefaultForeground
 *  borderWidth		BorderWidth	Dimension	1
 *  height		Height		Dimension	0
 *  reportCallback	ReportCallback	Pointer		NULL
 *  width		Width		Dimension	0
 *  x 			Position	Position	0
 *  y			Position	Position	0
 * 
 *****************************************************************************/

					/* external declarations */

extern WidgetClass portholeWidgetClass;
typedef struct _PortholeClassRec *PortholeWidgetClass;
typedef struct _PortholeRec      *PortholeWidget;

#endif /* _XawPorthole_h */
