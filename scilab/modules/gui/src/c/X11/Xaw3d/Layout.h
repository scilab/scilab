/*
 * $XConsortium: Layout.h,v 1.2 92/01/22 18:03:05 keith Exp $
 *
 * Copyright 1991 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, MIT X Consortium
 */


#ifndef _XawLayout_h
#define _XawLayout_h

#include <X11/Constraint.h>
#include <X11/Xfuncproto.h>

/****************************************************************
 *
 * Layout Widget (SubClass of CompositeClass)
 *
 ****************************************************************/

/* RESOURCES:

 Name		         Class		   RepType	    Default Value
 ----		         -----		   -------	    -------------
 background	         Background	   Pixel	    XtDefaultBackground
 border		         BorderColor       Pixel	    XtDefaultForeground
 borderWidth	         BorderWidth       Dimension	    1
 cursor		         Cursor	           Cursor	    None
 destroyCallback         Callback	   Pointer	    NULL
 height		         Height	           Dimension	    0
 mappedWhenManaged       MappedWhenManaged Boolean	    True
 sensitive	         Sensitive	   Boolean	    True
 width		         Width	           Dimension	    0
 x		         Position	   Position	    0
 y		         Position	   Position    	    0
 layout			 Layout		   Layout	    NULL

*/

/*
 * Syntax of layout resource
 *
 *  *layout:\
 *	<widget-name>.<edge>,<widget-name>.<edge>: distance + stretch-factor\n\
 *	...
 *  where the null widget-name is taken to be the Layout widget
 *
 * e.g:
 *
 *  *label-1.hStretch: 0
 *  *label-2.vStretch: 1
 *  *layout:\
 *	.left,		label-1.left:	10 + 0\n\
 *	label-1.right,	label-2.left:	10 + 1\n\
 *	label-2.right,	.right:		10 + 0
 *
 * This layout causes label-1 to be set 10 pixels from the left edge
 * and be whatever size the label widget requests, while label-2 will
 * be set 10 pixels from the right edge, and take up half of the remaining
 * space to 10 pixels from the right edge of label-1.
 */
 
/* New Fields */
#define XtNlayout "layout"
#define XtCLayout "Layout"
#define XtRLayout "Layout"
#define XtNdebug  "debug"

/* Class record constant */

extern WidgetClass layoutWidgetClass;

typedef struct _LayoutClassRec	*LayoutWidgetClass;
typedef struct _LayoutRec	*LayoutWidget;

#endif /* _Layout_h */
