/* $XConsortium: ShapeWidg.c,v 1.5 90/12/20 11:01:29 converse Exp $ */

/* 
 * Copyright 1988 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided 
 * that the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission. M.I.T. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 */

#include <X11/IntrinsicP.h>
#include <X11/extensions/shape.h>
#include "Converters.h"
#include "Drawing.h"

static ShapeError();
static  ShapeRectangle(), ShapeOval(), ShapeEllipseOrRoundedRectangle();

Boolean XmuReshapeWidget(w, shape_style, corner_width, corner_height)
    Widget w;
    int shape_style;
    int corner_width, corner_height;
{
    switch (shape_style) {

      case XmuShapeRectangle:
	ShapeRectangle(w);
	break;

      case XmuShapeOval:
	ShapeOval(w);
	break;

      case XmuShapeEllipse:
      case XmuShapeRoundedRectangle:
	ShapeEllipseOrRoundedRectangle
	    (w,
	     ((shape_style == XmuShapeEllipse) ? True : False),
	     corner_width,
	     corner_height);
	break;

      default:
	ShapeError(w);
	return False;
    }
    return True;
}

static ShapeError(w)
    Widget w;
{
    String params[1];
    Cardinal num_params = 1;
    params[0] = XtName(w);
    XtAppWarningMsg( XtWidgetToApplicationContext(w),
		     "shapeUnknown", "xmuReshapeWidget", "XmuLibrary",
		     "Unsupported shape style for Command widget \"%s\"",
		     params, &num_params
		   );
}


static ShapeRectangle(w)
    Widget w;
{
    XShapeCombineMask( XtDisplay(w), XtWindow(w),
		       ShapeBounding, 0, 0, None, ShapeSet );
    XShapeCombineMask( XtDisplay(w), XtWindow(w),
		       ShapeClip, 0, 0, None, ShapeSet );
}


static ShapeOval(w)
    Widget w;
{
    Display *dpy = XtDisplay(w);
    unsigned width = w->core.width + (w->core.border_width<<1);
    unsigned height = w->core.height + (w->core.border_width<<1);
    Pixmap p = XCreatePixmap( dpy, XtWindow(w), width, height, 1 );
    XGCValues values;
    GC gc;
    int rad;

    values.foreground = 0;
    values.background = 1;
    values.cap_style = CapRound;
    values.line_width = height;
    gc = XCreateGC (dpy, p,
		    GCForeground | GCBackground | GCLineWidth | GCCapStyle,
		    &values);
    XFillRectangle( dpy, p, gc, 0, 0, width, height );
    XSetForeground( dpy, gc, 1 );
    if (width <= height) {
	/* cannot be oval, fall back to ellipse */
	XFillArc( dpy, p, gc, 0, 0, width, height, 0, 360*64 );
    } else {
	rad = height >> 1;
	XDrawLine( dpy, p, gc, rad, rad, (int)width - rad - 1, rad );
    }
    XShapeCombineMask( dpy, XtWindow(w), ShapeBounding, 
		       -(w->core.border_width), -(w->core.border_width),
		       p, ShapeSet );
    if (w->core.border_width) {
	XSetForeground( dpy, gc, 0 );
	XFillRectangle( dpy, p, gc, 0, 0, width, height );
	values.line_width = w->core.height;
	values.foreground = 1;
	XChangeGC (dpy, gc, GCLineWidth|GCForeground, &values);
	if (w->core.width <= w->core.height) {
	    /* cannot be oval, fall back to ellipse */
	    XFillArc( dpy, p, gc, 0, 0, w->core.width, w->core.height,
		      0, 360*64 );
	} else {
	    rad = w->core.height >> 1;
	    XDrawLine( dpy, p, gc, rad, rad,
		       (int)w->core.width - rad - 1, rad );
	}
	XShapeCombineMask( dpy, XtWindow(w), ShapeClip, 0, 0, p, ShapeSet );
    } else {
	XShapeCombineMask( XtDisplay(w), XtWindow(w),
			  ShapeClip, 0, 0, None, ShapeSet );
    }
    XFreePixmap( dpy, p );
    XFreeGC (dpy, gc );
}


static ShapeEllipseOrRoundedRectangle(w, ellipse, ew, eh)
    Widget w;
    Boolean ellipse;
    int ew, eh;
{
    Display *dpy = XtDisplay(w);
    unsigned width = w->core.width + (w->core.border_width<<1);
    unsigned height = w->core.height + (w->core.border_width<<1);
    Pixmap p = XCreatePixmap( dpy, XtWindow(w), width, height, 1 );
    XGCValues values;
    GC gc;

    values.foreground = 0;
    gc = XCreateGC (dpy, p, GCForeground, &values );
    XFillRectangle( dpy, p, gc, 0, 0, width, height );
    XSetForeground (dpy, gc, 1);
    if (!ellipse)
	XmuFillRoundedRectangle( dpy, p, gc, 0, 0, (int)width, (int)height,
				 ew, eh );
    else
	XFillArc( dpy, p, gc, 0, 0, width, height, 0, 360*64 );
    XShapeCombineMask( dpy, XtWindow(w), ShapeBounding, 
		       -(w->core.border_width), -(w->core.border_width),
		       p, ShapeSet );
    if (w->core.border_width) {
	XSetForeground (dpy, gc, 0);
	XFillRectangle( dpy, p, gc, 0, 0, width, height );
	XSetForeground (dpy, gc, 1);
	if (!ellipse)
	    XmuFillRoundedRectangle( dpy, p, gc, 0, 0,
				     (int)w->core.width, (int)w->core.height,
				     ew, eh );
	else
	    XFillArc( dpy, p, gc, 0, 0, w->core.width, w->core.height,
		      0, 360*64 );
	XShapeCombineMask( dpy, XtWindow(w), ShapeClip, 0, 0, p, ShapeSet );
    } else {
	XShapeCombineMask( XtDisplay(w), XtWindow(w),
			   ShapeClip, 0, 0, None, ShapeSet );
    }
    XFreePixmap( dpy, p );
    XFreeGC (dpy, gc);
}
