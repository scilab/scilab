/*
 * $XConsortium: DrRndRect.c,v 1.2 89/10/03 08:38:21 rws Exp $
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * XmuDrawRoundedRectangle, XmuFillRoundedRectangle
 *
 * Draw/Fill a rounded rectangle, where x, y, w, h are the dimensions of
 * the overall rectangle, and ew and eh are the sizes of a bounding box
 * that the corners are drawn inside of.
 */

#include <X11/Xlib.h>

void
XmuDrawRoundedRectangle (dpy, draw, gc, x, y, w, h, ew, eh)
    Display		*dpy;
    Drawable		draw;
    GC			gc;
    int			x, y, w, h, ew, eh;
{
	XArc	arcs[8];

	if (ew*2 > w)
	    ew = 0;
	if (eh*2 > h)
	    eh = 0;

	arcs[0].x = x;
	arcs[0].y = y;
	arcs[0].width = ew*2;
	arcs[0].height = eh*2;
	arcs[0].angle1 = 180*64;
	arcs[0].angle2 = -90*64;

	arcs[1].x = x + ew;
	arcs[1].y = y;
	arcs[1].width = w - ew*2;
	arcs[1].height = 0;
	arcs[1].angle1 = 180*64;
	arcs[1].angle2 = -180*64;

	arcs[2].x = x + w - ew*2;
	arcs[2].y = y;
	arcs[2].width = ew*2;
	arcs[2].height = eh*2;
	arcs[2].angle1 = 90*64;
	arcs[2].angle2 = -90*64;

	arcs[3].x = x + w;
	arcs[3].y = y + eh;
	arcs[3].width = 0;
	arcs[3].height = h - eh*2;
	arcs[3].angle1 = 90 * 64;
	arcs[3].angle2 = -180*64;

	arcs[4].x = x + w - ew*2;
	arcs[4].y = y + h - eh*2;
	arcs[4].width = ew * 2;
	arcs[4].height = eh * 2;
	arcs[4].angle1 = 0;
	arcs[4].angle2 = -90*64;

	arcs[5].x = x + ew;
	arcs[5].y = y + h;
	arcs[5].width = w - ew*2;
	arcs[5].height = 0;
	arcs[5].angle1 = 0;
	arcs[5].angle2 = -180*64;

	arcs[6].x = x;
	arcs[6].y = y + h - eh*2;
	arcs[6].width = ew*2;
	arcs[6].height = eh*2;
	arcs[6].angle1 = 270*64;
	arcs[6].angle2 = -90*64;

	arcs[7].x = x;
	arcs[7].y = y + eh;
	arcs[7].width = 0;
	arcs[7].height = h - eh*2;
	arcs[7].angle1 = 270*64;
	arcs[7].angle2 = -180*64;
	XDrawArcs (dpy, draw, gc, arcs, 8);
}

void
XmuFillRoundedRectangle (dpy, draw, gc, x, y, w, h, ew, eh)
    Display		*dpy;
    Drawable		draw;
    GC			gc;
    int			x, y, w, h, ew, eh;
{
	XArc	arcs[4];
	XRectangle rects[3];
	XGCValues vals;

	XGetGCValues(dpy, gc, GCArcMode, &vals);
	if (vals.arc_mode != ArcPieSlice)
	    XSetArcMode(dpy, gc, ArcPieSlice);

	if (ew*2 > w)
	    ew = 0;
	if (eh*2 > h)
	    eh = 0;

	arcs[0].x = x;
	arcs[0].y = y;
	arcs[0].width = ew*2;
	arcs[0].height = eh*2;
	arcs[0].angle1 = 180*64;
	arcs[0].angle2 = -90*64;

	arcs[1].x = x + w - ew*2;
	arcs[1].y = y;
	arcs[1].width = ew*2;
	arcs[1].height = eh*2;
	arcs[1].angle1 = 90*64;
	arcs[1].angle2 = -90*64;

	arcs[2].x = x + w - ew*2;
	arcs[2].y = y + h - eh*2;
	arcs[2].width = ew*2;
	arcs[2].height = eh*2;
	arcs[2].angle1 = 0;
	arcs[2].angle2 = -90*64;

	arcs[3].x = x;
	arcs[3].y = y + h - eh*2;
	arcs[3].width = ew*2;
	arcs[3].height = eh*2;
	arcs[3].angle1 = 270*64;
	arcs[3].angle2 = -90*64;

	XFillArcs (dpy, draw, gc, arcs, 4);

	rects[0].x = x + ew;
	rects[0].y = y;
	rects[0].width = w - ew*2;
	rects[0].height = h;

	rects[1].x = x;
	rects[1].y = y + eh;
	rects[1].width = ew;
	rects[1].height = h - eh*2;

	rects[2].x = x + w - ew;
	rects[2].y = y + eh;
	rects[2].width = ew;
	rects[2].height = h - eh*2;

	XFillRectangles (dpy, draw, gc, rects, 3);

	if (vals.arc_mode != ArcPieSlice)
	    XSetArcMode(dpy, gc, vals.arc_mode);
}
