
/* MODIFIED ATHENA SCROLLBAR (USING ARROWHEADS AT ENDS OF TRAVEL) */
/* Modifications Copyright 1992 by Mitch Trachtenberg             */
/* Rights, permissions, and disclaimer of warranty are as in the  */
/* DEC and MIT notice below.                                      */
/* $XConsortium: Scrollbar.c,v 1.70 91/10/16 21:39:40 eswu Exp $ */

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

/* ScrollBar.c */
/* created by weissman, Mon Jul  7 13:20:03 1986 */
/* converted by swick, Thu Aug 27 1987 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/ScrollbarP.h>

#include <X11/Xmu/Drawing.h>

/* Private definitions. */

#ifdef ARROW_SCROLLBAR
static char defaultTranslations[] =
    "<Btn1Down>:   NotifyScroll()\n\
     <Btn2Down>:   MoveThumb() NotifyThumb() \n\
     <Btn3Down>:   NotifyScroll()\n\
     <Btn1Motion>: HandleThumb() \n\
     <Btn3Motion>: HandleThumb() \n\
     <Btn2Motion>: MoveThumb() NotifyThumb() \n\
     <BtnUp>:      EndScroll()";
#else
static char defaultTranslations[] =
    "<Btn1Down>:   StartScroll(Forward) \n\
     <Btn2Down>:   StartScroll(Continuous) MoveThumb() NotifyThumb() \n\
     <Btn3Down>:   StartScroll(Backward) \n\
     <Btn2Motion>: MoveThumb() NotifyThumb() \n\
     <BtnUp>:      NotifyScroll(Proportional) EndScroll()";
#ifdef bogusScrollKeys
    /* examples */
    "<KeyPress>f:  StartScroll(Forward) NotifyScroll(FullLength) EndScroll()"
    "<KeyPress>b:  StartScroll(Backward) NotifyScroll(FullLength) EndScroll()"
#endif
#endif

static float floatZero = 0.0;

#define Offset(field) XtOffsetOf(ScrollbarRec, field)

static XtResource resources[] = {
#ifdef ARROW_SCROLLBAR
/*  {XtNscrollCursor, XtCCursor, XtRCursor, sizeof(Cursor),
       Offset(scrollbar.cursor), XtRString, "crosshair"},*/
#else
  {XtNscrollVCursor, XtCCursor, XtRCursor, sizeof(Cursor),
       Offset(scrollbar.verCursor), XtRString, "sb_v_double_arrow"},
  {XtNscrollHCursor, XtCCursor, XtRCursor, sizeof(Cursor),
       Offset(scrollbar.horCursor), XtRString, "sb_h_double_arrow"},
  {XtNscrollUCursor, XtCCursor, XtRCursor, sizeof(Cursor),
       Offset(scrollbar.upCursor), XtRString, "sb_up_arrow"},
  {XtNscrollDCursor, XtCCursor, XtRCursor, sizeof(Cursor),
       Offset(scrollbar.downCursor), XtRString, "sb_down_arrow"},
  {XtNscrollLCursor, XtCCursor, XtRCursor, sizeof(Cursor),
       Offset(scrollbar.leftCursor), XtRString, "sb_left_arrow"},
  {XtNscrollRCursor, XtCCursor, XtRCursor, sizeof(Cursor),
       Offset(scrollbar.rightCursor), XtRString, "sb_right_arrow"},
#endif
  {XtNlength, XtCLength, XtRDimension, sizeof(Dimension),
       Offset(scrollbar.length), XtRImmediate, (XtPointer) 1},
  {XtNthickness, XtCThickness, XtRDimension, sizeof(Dimension),
       Offset(scrollbar.thickness), XtRImmediate, (XtPointer) 14},
  {XtNorientation, XtCOrientation, XtROrientation, sizeof(XtOrientation),
      Offset(scrollbar.orientation), XtRImmediate, (XtPointer) XtorientVertical},
  {XtNscrollProc, XtCCallback, XtRCallback, sizeof(XtPointer),
       Offset(scrollbar.scrollProc), XtRCallback, NULL},
  {XtNthumbProc, XtCCallback, XtRCallback, sizeof(XtPointer),
       Offset(scrollbar.thumbProc), XtRCallback, NULL},
  {XtNjumpProc, XtCCallback, XtRCallback, sizeof(XtPointer),
       Offset(scrollbar.jumpProc), XtRCallback, NULL},
  {XtNthumb, XtCThumb, XtRBitmap, sizeof(Pixmap),
       Offset(scrollbar.thumb), XtRImmediate, (XtPointer) XtUnspecifiedPixmap},
  {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
       Offset(scrollbar.foreground), XtRString, XtDefaultForeground},
  {XtNshown, XtCShown, XtRFloat, sizeof(float),
       Offset(scrollbar.shown), XtRFloat, (XtPointer)&floatZero},
  {XtNtopOfThumb, XtCTopOfThumb, XtRFloat, sizeof(float),
       Offset(scrollbar.top), XtRFloat, (XtPointer)&floatZero},
  {XtNpickTop, XtCPickTop, XtRBoolean, sizeof(Boolean),
       Offset(scrollbar.pick_top), XtRBoolean, (XtPointer) False},
  {XtNminimumThumb, XtCMinimumThumb, XtRDimension, sizeof(Dimension),
       Offset(scrollbar.min_thumb), XtRImmediate, (XtPointer) 7}
};
#undef Offset

static void ClassInitialize();
static void Initialize();
static void Destroy();
static void Realize();
static void Resize();
static void Redisplay();
static Boolean SetValues();

#ifdef ARROW_SCROLLBAR
static void HandleThumb();
#else
static void StartScroll();
#endif
static void MoveThumb();
static void NotifyThumb();
static void NotifyScroll();
static void EndScroll();

static XtActionsRec actions[] = {
#ifdef ARROW_SCROLLBAR
    {"HandleThumb",	HandleThumb},
#else
    {"StartScroll",     StartScroll},
#endif
    {"MoveThumb",	MoveThumb},
    {"NotifyThumb",	NotifyThumb},
    {"NotifyScroll",	NotifyScroll},
    {"EndScroll",	EndScroll}
};


ScrollbarClassRec scrollbarClassRec = {
  { /* core fields */
    /* superclass       */	(WidgetClass) &threeDClassRec,
    /* class_name       */	"Scrollbar",
    /* size             */	sizeof(ScrollbarRec),
    /* class_initialize	*/	ClassInitialize,
    /* class_part_init  */	NULL,
    /* class_inited	*/	FALSE,
    /* initialize       */	Initialize,
    /* initialize_hook  */	NULL,
    /* realize          */	Realize,
    /* actions          */	actions,
    /* num_actions	*/	XtNumber(actions),
    /* resources        */	resources,
    /* num_resources    */	XtNumber(resources),
    /* xrm_class        */	NULLQUARK,
    /* compress_motion	*/	TRUE,
    /* compress_exposure*/	TRUE,
    /* compress_enterleave*/	TRUE,
    /* visible_interest */	FALSE,
    /* destroy          */	Destroy,
    /* resize           */	Resize,
    /* expose           */	Redisplay,
    /* set_values       */	SetValues,
    /* set_values_hook  */	NULL,
    /* set_values_almost */	XtInheritSetValuesAlmost,
    /* get_values_hook  */	NULL,
    /* accept_focus     */	NULL,
    /* version          */	XtVersion,
    /* callback_private */	NULL,
    /* tm_table         */	defaultTranslations,
    /* query_geometry	*/	XtInheritQueryGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension        */	NULL
  },
  { /* simple fields */
    /* change_sensitive	*/	XtInheritChangeSensitive
  },
  { /* threeD fields */
    /* shadowdraw	*/	XtInheritXaw3dShadowDraw /*,*/
    /* shadowboxdraw	*/	/*XtInheritXaw3dShadowBoxDraw*/
  },
  { /* scrollbar fields */
    /* ignore		*/	0
  }

};

WidgetClass scrollbarWidgetClass = (WidgetClass)&scrollbarClassRec;

#define NoButton -1
#define PICKLENGTH(widget, x, y) \
    ((widget->scrollbar.orientation == XtorientHorizontal) ? x : y)
#define MIN(x,y)	((x) < (y) ? (x) : (y))
#define MAX(x,y)	((x) > (y) ? (x) : (y))

static void ClassInitialize()
{
    XawInitializeWidgetSet();
    XtAddConverter( XtRString, XtROrientation, XmuCvtStringToOrientation,
		    (XtConvertArgList)NULL, (Cardinal)0 );
}

#ifdef ARROW_SCROLLBAR
/* CHECKIT #define MARGIN(sbw) (sbw)->scrollbar.thickness + (sbw)->threeD.shadow_width */
#define MARGIN(sbw) (sbw)->scrollbar.thickness
#else
#define MARGIN(sbw) (sbw)->threeD.shadow_width
#endif

/* 
 The original Xaw Scrollbar's FillArea *really* relied on the fact that the 
 server was going to clip at the window boundaries; so the logic was really
 rather sloppy.  To avoid drawing over the shadows and the arrows requires 
 some extra care...  Hope I didn't make any mistakes.
*/
static void FillArea (sbw, top, bottom, fill)
    ScrollbarWidget sbw;
    Position top, bottom;
    int fill;
{
    int tlen = bottom - top;	/* length of thumb in pixels */
    int sw, margin, floor;
    int lx, ly, lw, lh;

    if (bottom <= 0 || bottom <= top) 
	return;
    if ((sw = sbw->threeD.shadow_width) < 0) 
	sw = 0;
    margin = MARGIN (sbw);
    floor = sbw->scrollbar.length - margin;

    if (sbw->scrollbar.orientation == XtorientHorizontal) {
	lx = ((top < margin) ? margin : top);
	ly = sw;
	lw = ((bottom > floor) ? floor - top : tlen);
/* CHECKIT	lw = (((top + tlen) > floor) ? floor - top : tlen); */
	lh = sbw->core.height - 2 * sw;
    } else {
	lx = sw;
	ly = ((top < margin) ? margin : top);
	lw = sbw->core.width - 2 * sw;
/* CHECKIT	lh = (((top + tlen) > floor) ? floor - top : tlen); */
	lh = ((bottom > floor) ? floor - top : tlen);
    }
    if (lh <= 0 || lw <= 0) return;
    if (fill) {
	XFillRectangle(XtDisplay((Widget) sbw), XtWindow((Widget) sbw), 
			sbw->scrollbar.gc, 
			lx, ly, (unsigned int) lw, (unsigned int) lh);
    } else {
	XClearArea (XtDisplay((Widget) sbw), XtWindow((Widget) sbw), 
			lx, ly, (unsigned int) lw, (unsigned int) lh, 
			FALSE);
    }
}



/* _ShadowSurroundedBox is somewhat redundant with 
     _Xaw3dDrawShadows (gw, event, region, out)
  in ThreeD.c; however, it is more general and allows to
  specify the boxsize of the shawdowbox explicitely. In order
  to keep the changes in other parts of Xaw3d to Kaleb's distribution
  minimal, we define it here instead of as a method in the ThreeD class.

 -gustaf */

/* ARGSUSED */
static void
_ShadowSurroundedBox (gw, event, region, xtl, ytl, xbr, ybr, out)
    Widget gw;
    XEvent *event;
    Region region;
    Dimension  xtl, ytl, xbr, ybr; /* INNER corners */
    Boolean out;
{
    XPoint       pt[6];
    ThreeDWidget tdw = (ThreeDWidget) gw;
    Dimension    s   = tdw->threeD.shadow_width;

    /* 
     *  no point to do anything if the shadow_width is 0 or the
     *  widget has not been realized.
     */ 
    if((s > 0) && XtIsRealized (gw)){

        Dimension       xms = xtl - s;
        Dimension       yms = ytl - s;
        Dimension       xps = xbr + s;
        Dimension       yps = ybr + s;
        Display         *dpy = XtDisplay (gw);
        Window          win = XtWindow (gw);
        GC              top, bot;

        if (out) {
            top = tdw->threeD.top_shadow_GC;
            bot = tdw->threeD.bot_shadow_GC;
        } else {
            top = tdw->threeD.bot_shadow_GC;
            bot = tdw->threeD.top_shadow_GC;
        }
        /* Draw shadows. Points are numbered as follows:
         *
         *   1_________________________________ 2
         *   |                               / /|
         *   |                              / / |
         *   |   4_________________________/ /  |
         *   |   |                         3|   |

         *   |   |                          |   |
         *   |   |                          |   |
         *   |   |                          |   |
         *   |   |5_________________________|   |
         *   |  / /                         4   |
         *   | / /                              |
         *   |/ /_______________________________|
         *   3                                  1
         */

        /* points common to top and bottom shadow */
        pt[0].x = xms;  pt[0].y = yps;
        pt[2].x = xps;  pt[2].y = yms;
        pt[3].x = xbr;  pt[3].y = ytl;
        pt[5].x = xtl;  pt[5].y = ybr;

        /* top-left shadow */
        if ((region == NULL) ||
            (XRectInRegion (region, xms, yms, xps, ytl) != RectangleOut) ||
            (XRectInRegion (region, xms, yms, xtl, yps) != RectangleOut)) {

            pt[1].x = xms;      pt[1].y = yms;
            pt[4].x = xtl;      pt[4].y = ytl;
            XFillPolygon (dpy, win, top, pt, 6,Complex,CoordModeOrigin);
        }

        /* bottom-right shadow */
        if ((region == NULL) ||
            (XRectInRegion (region, xms, ybr, xps, yps) != RectangleOut) ||
            (XRectInRegion (region, xbr, yms, xps, yps) != RectangleOut)) {

            /* swap points from top left to bottom right */
            pt[1].x = xps;      pt[1].y = yps;
            pt[4].x = xbr;      pt[4].y = ybr;
            XFillPolygon (dpy, win, bot, pt,6, Complex,CoordModeOrigin);
        }
    }
}


/* Paint the thumb in the area specified by sbw->top and
   sbw->shown.  The old area is erased.  The painting and
   erasing is done cleverly so that no flickering will occur. */

static void PaintThumb (sbw, event)
    ScrollbarWidget sbw;
    XEvent *event; 
{
    Dimension s                   = sbw->threeD.shadow_width;
    Position  oldtop              = sbw->scrollbar.topLoc;
    Position  oldbot              = oldtop + sbw->scrollbar.shownLength;
    Dimension margin              = MARGIN (sbw);
    Dimension tzl                 = sbw->scrollbar.length - margin - margin;
    Position newtop, newbot;    
    Position  floor               = sbw->scrollbar.length - margin;

    newtop = margin + (int)(tzl * sbw->scrollbar.top);
    newbot = newtop + (int)(tzl * sbw->scrollbar.shown);
    if (sbw->scrollbar.shown < 1.) newbot++;
    if (newbot < newtop + (int)sbw->scrollbar.min_thumb +
                        2 * (int)sbw->threeD.shadow_width) 
      newbot = newtop + sbw->scrollbar.min_thumb +
                        2 * sbw->threeD.shadow_width;
    if ( newbot >= floor ) {
	newtop = floor-(newbot-newtop)+1;
	newbot = floor;
    }

    sbw->scrollbar.topLoc = newtop;
    sbw->scrollbar.shownLength = newbot - newtop;
    if (XtIsRealized ((Widget) sbw)) {
      /*  3D thumb wanted ?
       */
      if (s) 
	  {
          if (newtop < oldtop) FillArea(sbw, oldtop, oldtop + s, 0);
          if (newtop > oldtop) FillArea(sbw, oldtop, MIN(newtop, oldbot), 0);
          if (newbot < oldbot) FillArea(sbw, MAX(newbot, oldtop), oldbot, 0);
          if (newbot > oldbot) FillArea(sbw, oldbot - s, oldbot, 0);

          if (sbw->scrollbar.orientation == XtorientHorizontal) 
	      {
	      _ShadowSurroundedBox(
		  sbw, event,
		  (Region)NULL,
		  newtop + s, 2 * s,
		  newbot - s, sbw->core.height - 2 * s, TRUE);
	      }
	  else 
	      {
	      _ShadowSurroundedBox(
		  sbw, event,
		  (Region)NULL,
		  2 * s, newtop + s,
		  sbw->core.width - 2 * s, newbot - s,
		  TRUE);
	      }
	  }
      else 
	  {
	  /* 
	    Note to Mitch: FillArea is (now) correctly implemented to 
	    not draw over shadows or the arrows. Therefore setting clipmasks 
	    doesn't seem to be necessary.  Correct me if I'm wrong!
	  */
          if (newtop < oldtop) FillArea(sbw, newtop, MIN(newbot, oldtop), 1);
          if (newtop > oldtop) FillArea(sbw, oldtop, MIN(newtop, oldbot), 0);
          if (newbot < oldbot) FillArea(sbw, MAX(newbot, oldtop), oldbot, 0);
          if (newbot > oldbot) FillArea(sbw, MAX(newtop, oldbot), newbot, 1);
	  }
    }
}

#ifdef ARROW_SCROLLBAR
static void PaintArrows (sbw)
    ScrollbarWidget sbw;
{
    XPoint    pt[20];
    Dimension s   = sbw->threeD.shadow_width;
    Dimension t   = sbw->scrollbar.thickness;
    Dimension l   = sbw->scrollbar.length;
    Dimension tms = t - s, lms = l - s;
    Dimension tm1 = t - 1;
    Dimension lmt = l - t;
    Dimension lp1 = lmt + 1;
    Dimension sm1 = s - 1;
    Dimension t2  = t / 2;
    Dimension sa30 = (Dimension)(1.732 * s );  /* cotangent of 30 deg */
    Display   *dpy = XtDisplay (sbw);
    Window    win = XtWindow (sbw);
    GC        top = sbw->threeD.top_shadow_GC;
    GC        bot = sbw->threeD.bot_shadow_GC;
  

    if (XtIsRealized ((Widget) sbw)) {
	/* 3D arrows?
         */
	if (s) {
	    /* upper/right arrow */
	    pt[0].x = sm1;         pt[0].y = tm1;
	    pt[1].x = t2;          pt[1].y = sm1;
	    pt[2].x = t2;          pt[2].y = s + sa30;
	    pt[3].x = sm1 + sa30;  pt[3].y = tms - 1;

	    pt[4].x = sm1;         pt[4].y = tm1;
	    pt[5].x = tms;         pt[5].y = tm1;
	    pt[6].x = t2;          pt[6].y = sm1;
	    pt[7].x = t2;          pt[7].y = s + sa30;
	    pt[8].x = tms - sa30;  pt[8].y = tms - 1;
	    pt[9].x = sm1 + sa30;  pt[9].y = tms - 1;

	    /* lower/left arrow */
	    pt[10].x = tms;        pt[10].y = lp1;
	    pt[11].x = s;          pt[11].y = lp1;
	    pt[12].x = t2;         pt[12].y = lms;
	    pt[13].x = t2;         pt[13].y = lms - sa30;
	    pt[14].x = s + sa30;   pt[14].y = lmt + s + 1;
	    pt[15].x = tms - sa30; pt[15].y = lmt + s + 1;

	    pt[16].x = tms;        pt[16].y = lp1;
	    pt[17].x = t2;         pt[17].y = lms;
	    pt[18].x = t2;         pt[18].y = lms - sa30;
	    pt[19].x = tms - sa30; pt[19].y = lmt + s + 1;

	    /* horizontal arrows require that x and y coordinates be swapped */
	    if (sbw->scrollbar.orientation == XtorientHorizontal) {
		int n;
		int swap;
		for (n = 0; n < 20; n++) {
		    swap = pt[n].x;
		    pt[n].x = pt[n].y;
		    pt[n].y = swap;
		}
	    }
	    XFillPolygon (dpy, win, top, pt, 4, Complex, CoordModeOrigin);
	    XFillPolygon (dpy, win, bot, pt + 4, 6, Complex, CoordModeOrigin);
	    XFillPolygon (dpy, win, top, pt + 10, 6, Complex, CoordModeOrigin);
	    XFillPolygon (dpy, win, bot, pt + 16, 4, Complex, CoordModeOrigin);

	} else {
	    pt[0].x = 0;      pt[0].y = tm1;
	    pt[1].x = t;      pt[1].y = tm1;
	    pt[2].x = t2;     pt[2].y = 0;
      
	    pt[3].x = 0;      pt[3].y = lp1;
	    pt[4].x = t;      pt[4].y = lp1;
	    pt[5].x = t2;     pt[5].y = l;

	    /* horizontal arrows require that x and y coordinates be swapped */
	    if (sbw->scrollbar.orientation == XtorientHorizontal) {
		int n;
		int swap;
		for (n = 0; n < 6; n++) {
		    swap = pt[n].x;
		    pt[n].x = pt[n].y;
		    pt[n].y = swap;
		}
	    }
	    /* draw the up/left arrow */
	    XFillPolygon (dpy, win, sbw->scrollbar.gc, 
			  pt, 3, 
			  Convex, CoordModeOrigin);
	    /* draw the down/right arrow */
	    XFillPolygon (dpy, win, sbw->scrollbar.gc, 
			  pt+3, 3, 
			  Convex, CoordModeOrigin);
	}
    }
}
#endif

/*	Function Name: Destroy
 *	Description: Called as the scrollbar is going away...
 *	Arguments: w - the scrollbar.
 *	Returns: nonw
 */
static void Destroy (w)
    Widget w;
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;
#ifdef ARROW_SCROLLBAR
    if(sbw->scrollbar.timer_id != (XtIntervalId) 0)
	XtRemoveTimeOut (sbw->scrollbar.timer_id);
#endif
    XtReleaseGC (w, sbw->scrollbar.gc);
}

/*	Function Name: CreateGC
 *	Description: Creates the GC.
 *	Arguments: w - the scrollbar widget.
 *	Returns: none. 
 */

static void CreateGC (w)
    Widget w;
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;
    XGCValues gcValues;
    XtGCMask mask;
    unsigned int depth = 1;

    if (sbw->scrollbar.thumb == XtUnspecifiedPixmap) {
        sbw->scrollbar.thumb = XmuCreateStippledPixmap (XtScreen(w), 
					(Pixel) 1, (Pixel) 0, depth);
    } else if (sbw->scrollbar.thumb != None) {
	Window root;
	int x, y;
	unsigned int width, height, bw;
	if (XGetGeometry (XtDisplay(w), sbw->scrollbar.thumb, &root, &x, &y,
			 &width, &height, &bw, &depth) == 0) {
	    XtAppError (XtWidgetToApplicationContext (w),
	       "Scrollbar Widget: Could not get geometry of thumb pixmap.");
	}
    }

    gcValues.foreground = sbw->scrollbar.foreground;
    gcValues.background = sbw->core.background_pixel;
    mask = GCForeground | GCBackground;

    if (sbw->scrollbar.thumb != None) {
	if (depth == 1) {
	    gcValues.fill_style = FillOpaqueStippled;
	    gcValues.stipple = sbw->scrollbar.thumb;
	    mask |= GCFillStyle | GCStipple;
	}
	else {
	    gcValues.fill_style = FillTiled;
	    gcValues.tile = sbw->scrollbar.thumb;
	    mask |= GCFillStyle | GCTile;
	}
    }
    /* the creation should be non-caching, because */
    /* we now set and clear clip masks on the gc returned */
    sbw->scrollbar.gc = XtGetGC (w, mask, &gcValues);
}

static void SetDimensions (sbw)
    ScrollbarWidget sbw;
{
    if (sbw->scrollbar.orientation == XtorientVertical) {
	sbw->scrollbar.length = sbw->core.height;
	sbw->scrollbar.thickness = sbw->core.width;
    } else {
	sbw->scrollbar.length = sbw->core.width;
	sbw->scrollbar.thickness = sbw->core.height;
    }
}

/* ARGSUSED */
static void Initialize( request, new, args, num_args )
    Widget request;		/* what the client asked for */
    Widget new;			/* what we're going to give him */
    ArgList args;
    Cardinal *num_args;
{
    ScrollbarWidget sbw = (ScrollbarWidget) new;

    CreateGC (new);

    if (sbw->core.width == 0)
	sbw->core.width = (sbw->scrollbar.orientation == XtorientVertical)
	    ? sbw->scrollbar.thickness : sbw->scrollbar.length;

    if (sbw->core.height == 0)
	sbw->core.height = (sbw->scrollbar.orientation == XtorientHorizontal)
	    ? sbw->scrollbar.thickness : sbw->scrollbar.length;

    SetDimensions (sbw);
#ifdef ARROW_SCROLLBAR
    sbw->scrollbar.scroll_mode = 0;
    sbw->scrollbar.timer_id = (XtIntervalId)0;
#endif
    sbw->scrollbar.topLoc = 0;
    sbw->scrollbar.shownLength = sbw->scrollbar.min_thumb;
}

static void Realize (w, valueMask, attributes)
    Widget w;
    Mask *valueMask;
    XSetWindowAttributes *attributes;
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;
#ifdef ARROW_SCROLLBAR
    if(sbw->simple.cursor_name == NULL)
	XtVaSetValues(w, XtNcursorName, "crosshair", NULL);
    /* dont set the cursor of the window to anything */
    *valueMask &= ~CWCursor;
#else
    sbw->scrollbar.inactiveCursor =
      (sbw->scrollbar.orientation == XtorientVertical)
	? sbw->scrollbar.verCursor
	: sbw->scrollbar.horCursor;

    XtVaSetValues (w, XtNcursor, sbw->scrollbar.inactiveCursor, NULL);
#endif
    /* 
     * The Simple widget actually stuffs the value in the valuemask. 
     */
    
    (*scrollbarWidgetClass->core_class.superclass->core_class.realize)
	(w, valueMask, attributes);
}

/* ARGSUSED */
static Boolean SetValues (current, request, desired, args, num_args)
    Widget  current,		/* what I am */
	    request,		/* what he wants me to be */
	    desired;		/* what I will become */
    ArgList args;
    Cardinal *num_args;
{
    ScrollbarWidget sbw = (ScrollbarWidget) current;
    ScrollbarWidget dsbw = (ScrollbarWidget) desired;
    Boolean redraw = FALSE;

/*
 * If these values are outside the acceptable range ignore them...
 */

    if (dsbw->scrollbar.top < 0.0 || dsbw->scrollbar.top > 1.0)
        dsbw->scrollbar.top = sbw->scrollbar.top;

    if (dsbw->scrollbar.shown < 0.0 || dsbw->scrollbar.shown > 1.0)
        dsbw->scrollbar.shown = sbw->scrollbar.shown;

/*
 * Change colors and stuff...
 */
    if (XtIsRealized (desired)) {
	if (sbw->scrollbar.foreground != dsbw->scrollbar.foreground ||
	    sbw->core.background_pixel != dsbw->core.background_pixel ||
	    sbw->scrollbar.thumb != dsbw->scrollbar.thumb) {
	    XtReleaseGC (desired, sbw->scrollbar.gc);
	    CreateGC (desired);
	    redraw = TRUE;
	}
	if (sbw->scrollbar.top != dsbw->scrollbar.top ||
	    sbw->scrollbar.shown != dsbw->scrollbar.shown)
	    redraw = TRUE;
    }
    return redraw;
}

static void Resize (w)
    Widget w;
{
    /* ForgetGravity has taken care of background, but thumb may
     * have to move as a result of the new size. */
    SetDimensions ((ScrollbarWidget) w);
    Redisplay (w, (XEvent*) NULL, (Region)NULL);
}


/* ARGSUSED */
static void Redisplay (w, event, region)
    Widget w;
    XEvent *event;
    Region region;
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;
    ScrollbarWidgetClass swclass = (ScrollbarWidgetClass) XtClass (w);
    int x, y;
    unsigned int width, height;

    (*swclass->threeD_class.shadowdraw) (w, event, region, FALSE);

    if (sbw->scrollbar.orientation == XtorientHorizontal) {
	x = sbw->scrollbar.topLoc;
	y = 1;
	width = sbw->scrollbar.shownLength;
	height = sbw->core.height - 2;
    } else {
	x = 1;
	y = sbw->scrollbar.topLoc;
	width = sbw->core.width - 2;
	height = sbw->scrollbar.shownLength;
    }
    if (region == NULL ||
	XRectInRegion (region, x, y, width, height) != RectangleOut) {
	/* Forces entire thumb to be painted. */
	sbw->scrollbar.topLoc = -(sbw->scrollbar.length + 1);
	PaintThumb (sbw, event); 
    }
#ifdef ARROW_SCROLLBAR
    /* we'd like to be region aware here!!!! */
    PaintArrows (sbw);
#endif

}


static Boolean CompareEvents (oldEvent, newEvent)
    XEvent *oldEvent, *newEvent;
{
#define Check(field) if (newEvent->field != oldEvent->field) return False;

    Check(xany.display);
    Check(xany.type);
    Check(xany.window);

    switch (newEvent->type) {
    case MotionNotify:
	Check(xmotion.state); 
	break;
    case ButtonPress:
    case ButtonRelease:
	Check(xbutton.state);
	Check(xbutton.button); 
	break;
    case KeyPress:
    case KeyRelease:
	Check(xkey.state);
	Check(xkey.keycode); 
	break;
    case EnterNotify:
    case LeaveNotify:
	Check(xcrossing.mode);
	Check(xcrossing.detail);
	Check(xcrossing.state); 
	break;
    }
#undef Check

    return True;
}

struct EventData {
    XEvent *oldEvent;
    int count;
};

static Bool PeekNotifyEvent (dpy, event, args)
    Display *dpy;
    XEvent *event;
    char *args;
{
    struct EventData *eventData = (struct EventData*)args;

    return ((++eventData->count == QLength(dpy)) /* since PeekIf blocks */
	    || CompareEvents(event, eventData->oldEvent));
}


static Boolean LookAhead (w, event)
    Widget w;
    XEvent *event;
{
    XEvent newEvent;
    struct EventData eventData;

    if (QLength (XtDisplay (w)) == 0) return False;

    eventData.count = 0;
    eventData.oldEvent = event;

    XPeekIfEvent (XtDisplay (w), &newEvent, PeekNotifyEvent, (char*)&eventData);

    return CompareEvents (event, &newEvent);
}


static void ExtractPosition (event, x, y)
    XEvent *event;
    Position *x, *y;		/* RETURN */
{
    switch( event->type ) {
    case MotionNotify:
	*x = event->xmotion.x;	 
	*y = event->xmotion.y;	  
	break;
    case ButtonPress:
    case ButtonRelease:
	*x = event->xbutton.x;   
	*y = event->xbutton.y;   
	break;
    case KeyPress:
    case KeyRelease:
	*x = event->xkey.x;      
	*y = event->xkey.y;	  
	break;
    case EnterNotify:
    case LeaveNotify:
	*x = event->xcrossing.x; 
	*y = event->xcrossing.y; 
	break;
    default:
	*x = 0; *y = 0;
    }
}

#ifdef ARROW_SCROLLBAR
/* ARGSUSED */
static void HandleThumb (w, event, params, num_params)
    Widget w;
    XEvent *event;
    String *params;		/* unused */
    Cardinal *num_params;	/* unused */
{
    Position x,y;
    ScrollbarWidget sbw = (ScrollbarWidget) w;

    ExtractPosition( event, &x, &y );
    /* if the motion event puts the pointer in thumb, call Move and Notify */
    /* also call Move and Notify if we're already in continuous scroll mode */
    if (sbw->scrollbar.scroll_mode == 2 ||
	(PICKLENGTH (sbw,x,y) >= sbw->scrollbar.topLoc &&
	PICKLENGTH (sbw,x,y) <= sbw->scrollbar.topLoc + sbw->scrollbar.shownLength)){
	XtCallActionProc(w, "MoveThumb", event, params, *num_params);
	XtCallActionProc(w, "NotifyThumb", event, params, *num_params);
    }
}

static void RepeatNotify (client_data, idp)
    XtPointer client_data;
    XtIntervalId *idp;
{
#define A_FEW_PIXELS 5
    ScrollbarWidget sbw = (ScrollbarWidget) client_data;
    int call_data;
    if (sbw->scrollbar.scroll_mode != 1 && sbw->scrollbar.scroll_mode != 3) {
	sbw->scrollbar.timer_id = (XtIntervalId) 0;
	return;
    }
    call_data = MAX (A_FEW_PIXELS, sbw->scrollbar.length / 20);
    if (sbw->scrollbar.scroll_mode == 1)
	call_data = -call_data;
    XtCallCallbacks((Widget)sbw, XtNscrollProc, (XtPointer) call_data);
    sbw->scrollbar.timer_id = 
    XtAppAddTimeOut(XtWidgetToApplicationContext((Widget)sbw),
		    (unsigned long) 150,
		    RepeatNotify,
		    client_data);
}

#else /* ARROW_SCROLLBAR */
/* ARGSUSED */
static void StartScroll (w, event, params, num_params )
    Widget w;
    XEvent *event;
    String *params;		/* direction: Back|Forward|Smooth */
    Cardinal *num_params;	/* we only support 1 */
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;
    Cursor cursor;
    char direction;

    if (sbw->scrollbar.direction != 0) return; /* if we're already scrolling */
    if (*num_params > 0) 
	direction = *params[0];
    else
	direction = 'C';

    sbw->scrollbar.direction = direction;

    switch (direction) {
    case 'B':
    case 'b':	
	cursor = (sbw->scrollbar.orientation == XtorientVertical)
			? sbw->scrollbar.downCursor
			: sbw->scrollbar.rightCursor; 
	break;
    case 'F':
    case 'f':
	cursor = (sbw->scrollbar.orientation == XtorientVertical)
			? sbw->scrollbar.upCursor
			: sbw->scrollbar.leftCursor; 
	break;
    case 'C':
    case 'c':
	cursor = (sbw->scrollbar.orientation == XtorientVertical)
			? sbw->scrollbar.rightCursor
			: sbw->scrollbar.upCursor; 
	break;
    default:	
	return; /* invalid invocation */
    }
    XtVaSetValues (w, XtNcursor, cursor, NULL);
    XFlush (XtDisplay (w));
}
#endif /* ARROW_SCROLLBAR */

/*
 * Make sure the first number is within the range specified by the other
 * two numbers.
 */

#ifndef ARROW_SCROLLBAR
static int InRange(num, small, big)
    int num, small, big;
{
    return (num < small) ? small : ((num > big) ? big : num);
}
#endif

/*
 * Same as above, but for floating numbers. 
 */

static float FloatInRange(num, small, big)
    float num, small, big;
{
    return (num < small) ? small : ((num > big) ? big : num);
}


#ifdef ARROW_SCROLLBAR
static void NotifyScroll (w, event, params, num_params)
    Widget w;
    XEvent *event;
    String *params;		
    Cardinal *num_params;	
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;
    int call_data;
    Position x, y;

    if (sbw->scrollbar.scroll_mode == 2  /* if scroll continuous */
	|| LookAhead (w, event)) 
	return;

    ExtractPosition (event, &x, &y);

    if (PICKLENGTH (sbw,x,y) < sbw->scrollbar.thickness) {
	/* handle first arrow zone */
	call_data = -MAX (A_FEW_PIXELS, sbw->scrollbar.length / 20);
	XtCallCallbacks (w, XtNscrollProc, (XtPointer)(call_data));
	/* establish autoscroll */
	sbw->scrollbar.timer_id = 
	    XtAppAddTimeOut (XtWidgetToApplicationContext (w),
				(unsigned long) 300, RepeatNotify, (XtPointer)w);
	sbw->scrollbar.scroll_mode = 1;
    } else if (PICKLENGTH (sbw,x,y) > sbw->scrollbar.length - sbw->scrollbar.thickness) {
	/* handle last arrow zone */
	call_data = MAX (A_FEW_PIXELS, sbw->scrollbar.length / 20);
	XtCallCallbacks (w, XtNscrollProc, (XtPointer)(call_data));
	/* establish autoscroll */
	sbw->scrollbar.timer_id =
	    XtAppAddTimeOut (XtWidgetToApplicationContext (w), 
				(unsigned long) 300, RepeatNotify, (XtPointer)w);
	sbw->scrollbar.scroll_mode = 3;
    } else if (PICKLENGTH (sbw, x, y) < sbw->scrollbar.topLoc) {
	/* handle zone "above" the thumb */
	call_data = - sbw->scrollbar.length;
	XtCallCallbacks (w, XtNscrollProc, (XtPointer)(call_data));
    } else if (PICKLENGTH (sbw, x, y) > sbw->scrollbar.topLoc + sbw->scrollbar.shownLength) {
	/* handle zone "below" the thumb */
	call_data = sbw->scrollbar.length;
	XtCallCallbacks (w, XtNscrollProc, (XtPointer)(call_data));
    } else 
	{
	/* handle the thumb in the motion notify action */
	}
    return;
}
#else /* ARROW_SCROLLBAR */
static void NotifyScroll (w, event, params, num_params)
    Widget w;
    XEvent *event;
    String *params;		/* style: Proportional|FullLength */
    Cardinal *num_params;	/* we only support 1 */
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;
    int call_data;
    char style;
    Position x, y;

    if (sbw->scrollbar.direction == 0) return; /* if no StartScroll */
    if (LookAhead (w, event)) return;
    if (*num_params > 0) 
	style = *params[0];
    else
	style = 'P';

    switch (style) {
    case 'P':    /* Proportional */
    case 'p':
	ExtractPosition (event, &x, &y);
	call_data = 
	    InRange (PICKLENGTH (sbw, x, y), 0, (int) sbw->scrollbar.length); 
	break;

    case 'F':    /* FullLength */
    case 'f':    
	call_data = sbw->scrollbar.length; 
	break;
    }
    switch (sbw->scrollbar.direction) {
    case 'B':
    case 'b':    
	call_data = -call_data;
	/* fall through */

    case 'F':
    case 'f':    
	XtCallCallbacks (w, XtNscrollProc, (XtPointer)call_data);
	break;

    case 'C':
    case 'c':
	/* NotifyThumb has already called the thumbProc(s) */
	break;
    }
}
#endif /* ARROW_SCROLLBAR */

/* ARGSUSED */
static void EndScroll(w, event, params, num_params )
    Widget w;
    XEvent *event;		/* unused */
    String *params;		/* unused */
    Cardinal *num_params;	/* unused */
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;

#ifdef ARROW_SCROLLBAR
    sbw->scrollbar.scroll_mode = 0;
    /* no need to remove any autoscroll timeout; it will no-op */
    /* because the scroll_mode is 0 */
    /* but be sure to remove timeout in destroy proc */
#else
    XtVaSetValues (w, XtNcursor, sbw->scrollbar.inactiveCursor, NULL);
    XFlush (XtDisplay (w));
    sbw->scrollbar.direction = 0;
#endif
}

static float FractionLoc (sbw, x, y)
    ScrollbarWidget sbw;
    int x, y;
{
    float   result;
    int margin;
    float   height, width;

    margin = MARGIN (sbw);
    x -= margin;
    y -= margin;
    height = sbw->core.height - 2 * margin;
    width = sbw->core.width - 2 * margin;
    result = PICKLENGTH (sbw, x / width, y / height);
    return FloatInRange(result, 0.0, 1.0);
}


static void MoveThumb (w, event, params, num_params)
    Widget w;
    XEvent *event;
    String *params;		/* unused */
    Cardinal *num_params;	/* unused */
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;
    Position x, y;
    float loc, t, s;

#ifndef ARROW_SCROLLBAR
    if (sbw->scrollbar.direction == 0) return; /* if no StartScroll */
#endif

    if (LookAhead (w, event)) return;

    if (!event->xmotion.same_screen) return;

    ExtractPosition (event, &x, &y);
    loc = FractionLoc (sbw, x, y);
    t = sbw->scrollbar.top;
    s = sbw->scrollbar.shown;
#ifdef ARROW_SCROLLBAR
    if (sbw->scrollbar.scroll_mode != 2 )
      /* initialize picked position */
      sbw->scrollbar.picked = (FloatInRange( loc, t, t + s ) - t);
#else
    sbw->scrollbar.picked = 0.5 * s;
#endif
    if (sbw->scrollbar.pick_top)
      sbw->scrollbar.top = loc;
    else {
      sbw->scrollbar.top = loc - sbw->scrollbar.picked;
      if (sbw->scrollbar.top < 0.0) sbw->scrollbar.top = 0.0;
    }

    if (sbw->scrollbar.top + sbw->scrollbar.shown > 1.0)
      sbw->scrollbar.top = 1.0 - sbw->scrollbar.shown;
#ifdef ARROW_SCROLLBAR
    sbw->scrollbar.scroll_mode = 2; /* indicate continuous scroll */
#endif
    PaintThumb (sbw, event);
    XFlush (XtDisplay (w));	/* re-draw it before Notifying */
}


/* ARGSUSED */
static void NotifyThumb (w, event, params, num_params )
    Widget w;
    XEvent *event;
    String *params;		/* unused */
    Cardinal *num_params;	/* unused */
{
    register ScrollbarWidget sbw = (ScrollbarWidget) w;
    float top = sbw->scrollbar.top;

#ifndef ARROW_SCROLLBAR
    if (sbw->scrollbar.direction == 0) return; /* if no StartScroll */
#endif

    if (LookAhead (w, event)) return;

    /* thumbProc is not pretty, but is necessary for backwards
       compatibility on those architectures for which it work{s,ed};
       the intent is to pass a (truncated) float by value. */
#ifdef ARROW_SCROLLBAR
    /* This corrects for rounding errors: If the thumb is moved to the end of
       the scrollable area sometimes the last line/column is not displayed.
       This can happen when the integer number of the top line or leftmost
       column to be be displayed is calculated from the float value
       sbw->scrollbar.top. The numerical error of this rounding problem is
       very small. We therefore add a small value which then forces the
       next line/column (the correct one) to be used. Since we can expect
       that the resolution of display screens will not be higher then
       10000 text lines/columns we add 1/10000 to the top position. The
       intermediate variable `top' is used to avoid erroneous summing up
       corrections (can this happen at all?). If the arrows are not displayed
       there is no problem since in this case there is always a constant
       integer number of pixels the thumb must be moved in order to scroll
       to the next line/column. */
    top += 0.0001;
#endif
    XtCallCallbacks (w, XtNthumbProc, *(XtPointer*)&top);
    XtCallCallbacks (w, XtNjumpProc, (XtPointer)&top);
}



/************************************************************
 *
 *  Public routines. 
 *
 ************************************************************/

/* Set the scroll bar to the given location. */

#if NeedFunctionPrototypes
void XawScrollbarSetThumb (Widget w, 
#if NeedWidePrototypes
			double top, double shown)
#else
			float top, float shown)
#endif
#else
void XawScrollbarSetThumb (w, top, shown)
    Widget w;
    float top, shown;
#endif
{
    ScrollbarWidget sbw = (ScrollbarWidget) w;

#ifdef WIERD
    fprintf(stderr,"< XawScrollbarSetThumb w=%p, top=%f, shown=%f\n", 
	    w,top,shown);
#endif

#ifdef ARROW_SCROLLBAR
    if (sbw->scrollbar.scroll_mode == (char) 2) return; /* if still thumbing */
#else
    if (sbw->scrollbar.direction == 'c') return; /* if still thumbing */
#endif

    sbw->scrollbar.top = (top > 1.0) ? 1.0 :
				(top >= 0.0) ? top : sbw->scrollbar.top;

    sbw->scrollbar.shown = (shown > 1.0) ? 1.0 :
				(shown >= 0.0) ? shown : sbw->scrollbar.shown;

    PaintThumb (sbw);
}

