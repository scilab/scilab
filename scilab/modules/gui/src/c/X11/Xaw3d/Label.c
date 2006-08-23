/* $XConsortium: Label.c,v 1.97 94/04/17 20:12:12 kaleb Exp $ */

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
 * Label.c - Label widget
 *
 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>
#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/LabelP.h>
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/Drawing.h>
#include <stdio.h>
#include <ctype.h>

/* needed for abs() */
#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#else
int abs();
#endif

#define streq(a,b) (strcmp( (a), (b) ) == 0)

#define MULTI_LINE_LABEL 32767

#ifdef CRAY
#define WORD64
#endif

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

#define offset(field) XtOffsetOf(LabelRec, field)
static XtResource resources[] = {
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	offset(label.foreground), XtRString, XtDefaultForeground},
    {XtNfont,  XtCFont, XtRFontStruct, sizeof(XFontStruct *),
	offset(label.font),XtRString, XtDefaultFont},
    {XtNfontSet,  XtCFontSet, XtRFontSet, sizeof(XFontSet ),
        offset(label.fontset),XtRString, XtDefaultFontSet},
    {XtNlabel,  XtCLabel, XtRString, sizeof(String),
	offset(label.label), XtRString, NULL},
    {XtNencoding, XtCEncoding, XtRUnsignedChar, sizeof(unsigned char),
	offset(label.encoding), XtRImmediate, (XtPointer)XawTextEncoding8bit},
    {XtNjustify, XtCJustify, XtRJustify, sizeof(XtJustify),
	offset(label.justify), XtRImmediate, (XtPointer)XtJustifyCenter},
    {XtNinternalWidth, XtCWidth, XtRDimension,  sizeof(Dimension),
	offset(label.internal_width), XtRImmediate, (XtPointer)4},
    {XtNinternalHeight, XtCHeight, XtRDimension, sizeof(Dimension),
	offset(label.internal_height), XtRImmediate, (XtPointer)2},
    {XtNleftBitmap, XtCLeftBitmap, XtRBitmap, sizeof(Pixmap),
       offset(label.left_bitmap), XtRImmediate, (XtPointer) None},
    {XtNbitmap, XtCPixmap, XtRBitmap, sizeof(Pixmap),
	offset(label.pixmap), XtRImmediate, (XtPointer)None},
    {XtNresize, XtCResize, XtRBoolean, sizeof(Boolean),
	offset(label.resize), XtRImmediate, (XtPointer)True},
    {XtNshadowWidth, XtCShadowWidth, XtRDimension, sizeof(Dimension),
	offset(threeD.shadow_width), XtRImmediate, (XtPointer) 0},
    {XtNborderWidth, XtCBorderWidth, XtRDimension, sizeof(Dimension),
         XtOffsetOf(RectObjRec,rectangle.border_width), XtRImmediate,
         (XtPointer)1}
};
#undef offset

static void Initialize();
static void Resize();
static void Redisplay();
static Boolean SetValues();
static void ClassInitialize();
static void Destroy();
static XtGeometryResult QueryGeometry();

LabelClassRec labelClassRec = {
  {
/* core_class fields */	
    /* superclass	  	*/	(WidgetClass) &threeDClassRec,
    /* class_name	  	*/	"Label",
    /* widget_size	  	*/	sizeof(LabelRec),
    /* class_initialize   	*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize		  	*/	XtInheritRealize,
    /* actions		  	*/	NULL,
    /* num_actions	  	*/	0,
    /* resources	  	*/	resources,
    /* num_resources	  	*/	XtNumber(resources),
    /* xrm_class	  	*/	NULLQUARK,
    /* compress_motion	  	*/	TRUE,
    /* compress_exposure  	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest	  	*/	FALSE,
    /* destroy		  	*/	Destroy,
    /* resize		  	*/	Resize,
    /* expose		  	*/	Redisplay,
    /* set_values	  	*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus	 	*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private   	*/	NULL,
    /* tm_table		   	*/	NULL,
    /* query_geometry		*/	QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
/* Simple class fields initialization */
  {
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
/* ThreeD class fields initialization */
  {
    /* shadowdraw 		*/	XtInheritXaw3dShadowDraw
  },
/* Label class fields initialization */
  {
    /* ignore 			*/	0
  }
};
WidgetClass labelWidgetClass = (WidgetClass)&labelClassRec;
/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static void ClassInitialize()
{
    XawInitializeWidgetSet();
    XtAddConverter( XtRString, XtRJustify, XmuCvtStringToJustify, 
		    (XtConvertArgList)NULL, 0 );
}

#ifndef WORD64

#define TXT16 XChar2b

#else

#define TXT16 char

static XChar2b *buf2b;
static int buf2blen = 0;

_XawLabelWidth16(fs, str, n)
    XFontStruct *fs;
    char *str;
    int	n;
{
    int i;
    XChar2b *ptr;

    if (n > buf2blen) {
	buf2b = (XChar2b *)XtRealloc((char *)buf2b, n * sizeof(XChar2b));
	buf2blen = n;
    }
    for (ptr = buf2b, i = n; --i >= 0; ptr++) {
	ptr->byte1 = *str++;
	ptr->byte2 = *str++;
    }
    return XTextWidth16(fs, buf2b, n);
}

_XawLabelDraw16(dpy, d, gc, x, y, str, n)
    Display *dpy;
    Drawable d;
    GC gc;
    int x, y;
    char *str;
    int n;
{
    int i;
    XChar2b *ptr;

    if (n > buf2blen) {
	buf2b = (XChar2b *)XtRealloc((char *)buf2b, n * sizeof(XChar2b));
	buf2blen = n;
    }
    for (ptr = buf2b, i = n; --i >= 0; ptr++) {
	ptr->byte1 = *str++;
	ptr->byte2 = *str++;
    }
    XDrawString16(dpy, d, gc, x, y, buf2b, n);
}

#define XTextWidth16 _XawLabelWidth16
#define XDrawString16 _XawLabelDraw16

#endif /* WORD64 */

/*
 * Calculate width and height of displayed text in pixels
 */

static void SetTextWidthAndHeight(lw)
    LabelWidget lw;
{
    XFontStruct	*fs = lw->label.font;

    char *nl;

    if (lw->label.pixmap != None) {
	Window root;
	int x, y;
	unsigned int width, height, bw, depth;
	if (XGetGeometry(XtDisplay(lw), lw->label.pixmap, &root, &x, &y,
			 &width, &height, &bw, &depth)) {
	    lw->label.label_height = height;
	    lw->label.label_width = width;
	    lw->label.label_len = depth;
	    return;
	}
    }
    if ( lw->simple.international == True ) {

      XFontSet	fset = lw->label.fontset;
      XFontSetExtents *ext = XExtentsOfFontSet(fset);

      lw->label.label_height = ext->max_ink_extent.height;
      if (lw->label.label == NULL) {
	  lw->label.label_len = 0;
	  lw->label.label_width = 0;
      }
      else if ((nl = index(lw->label.label, '\n')) != NULL) {
	  char *label;
	  lw->label.label_len = MULTI_LINE_LABEL;
	  lw->label.label_width = 0;
	  for (label = lw->label.label; nl != NULL; nl = index(label, '\n')) {
	      int width = XmbTextEscapement(fset, label, (int)(nl - label));

	      if (width > (int)lw->label.label_width)
		  lw->label.label_width = width;
	      label = nl + 1;
	      if (*label)
		  lw->label.label_height +=
		      ext->max_ink_extent.height;
	  }
	  if (*label) {
	      int width = XmbTextEscapement(fset, label, strlen(label));

	      if (width > (int) lw->label.label_width)
		  lw->label.label_width = width;
	  }
      } else {
	  lw->label.label_len = strlen(lw->label.label);
	  lw->label.label_width =
	      XmbTextEscapement(fset, lw->label.label, (int) lw->label.label_len);
      }

    } else {

        lw->label.label_height = fs->max_bounds.ascent + fs->max_bounds.descent;
        if (lw->label.label == NULL) {
            lw->label.label_len = 0;
            lw->label.label_width = 0;
        }
        else if ((nl = index(lw->label.label, '\n')) != NULL) {
	    char *label;
            lw->label.label_len = MULTI_LINE_LABEL;
            lw->label.label_width = 0;
            for (label = lw->label.label; nl != NULL; nl = index(label, '\n')) {
	        int width;

	        if (lw->label.encoding)
		    width = XTextWidth16(fs, (TXT16*)label, (int)(nl - label)/2);
	        else
		    width = XTextWidth(fs, label, (int)(nl - label));
	        if (width > (int)lw->label.label_width)
		    lw->label.label_width = width;
	        label = nl + 1;
	        if (*label)
		    lw->label.label_height +=
		        fs->max_bounds.ascent + fs->max_bounds.descent;
	    }
	    if (*label) {
	        int width = XTextWidth(fs, label, strlen(label));

	        if (lw->label.encoding)
		    width = XTextWidth16(fs, (TXT16*)label, (int)strlen(label)/2);
	        else
		    width = XTextWidth(fs, label, strlen(label));
	        if (width > (int) lw->label.label_width)
		    lw->label.label_width = width;
	    }
        } else {
	    lw->label.label_len = strlen(lw->label.label);
	    if (lw->label.encoding)
	        lw->label.label_width =
		    XTextWidth16(fs, (TXT16*)lw->label.label,
			         (int) lw->label.label_len/2);
	    else
	        lw->label.label_width =
		    XTextWidth(fs, lw->label.label, (int) lw->label.label_len);
        }

    }
}

static void GetnormalGC(lw)
    LabelWidget lw;
{
    XGCValues	values;

    values.foreground	= lw->label.foreground;
    values.background	= lw->core.background_pixel;
    values.font		= lw->label.font->fid;
    values.graphics_exposures = False;

    if ( lw->simple.international == True )
        /* Since Xmb/wcDrawString eats the font, I must use XtAllocateGC. */
        lw->label.normal_GC = XtAllocateGC(
                (Widget)lw, 0,
	(unsigned) GCForeground | GCBackground | GCGraphicsExposures,
	&values, GCFont, 0 );
    else
        lw->label.normal_GC = XtGetGC(
	(Widget)lw,
	(unsigned) GCForeground | GCBackground | GCFont | GCGraphicsExposures,
	&values);
}

static void GetgrayGC(lw)
    LabelWidget lw;
{
    XGCValues	values;

    values.foreground = lw->label.foreground;
    values.background = lw->core.background_pixel;
    values.font	      = lw->label.font->fid;
    values.fill_style = FillTiled;
    values.tile       = XmuCreateStippledPixmap(XtScreen((Widget)lw),
						lw->label.foreground, 
						lw->core.background_pixel,
						lw->core.depth);
    values.graphics_exposures = False;

    lw->label.stipple = values.tile;
    if ( lw->simple.international == True )
        /* Since Xmb/wcDrawString eats the font, I must use XtAllocateGC. */
        lw->label.gray_GC = XtAllocateGC((Widget)lw,  0,
				(unsigned) GCForeground | GCBackground |
					   GCTile | GCFillStyle |
					   GCGraphicsExposures,
				&values, GCFont, 0);
    else
        lw->label.gray_GC = XtGetGC((Widget)lw, 
				(unsigned) GCForeground | GCBackground |
					   GCFont | GCTile | GCFillStyle |
					   GCGraphicsExposures,
				&values);
}

static void compute_bitmap_offsets (lw)
    LabelWidget lw;
{
    /*
     * bitmap will be eventually be displayed at 
     * (internal_width, internal_height + lbm_y)
     */
    if (lw->label.lbm_height != 0) {
	lw->label.lbm_y = (lw->core.height -
			  (lw->threeD.shadow_width * 2 + 
			   lw->label.internal_height * 2 +
			   lw->label.lbm_height)) / 2;
    } else {
	lw->label.lbm_y = 0;
    }
}


static void set_bitmap_info (lw)
    LabelWidget lw;
{
    Window root;
    int x, y;
    unsigned int bw, depth;

    if (!(lw->label.left_bitmap &&
	  XGetGeometry (XtDisplay(lw), lw->label.left_bitmap, &root, &x, &y,
			&lw->label.lbm_width, &lw->label.lbm_height,
			&bw, &depth))) {
	lw->label.lbm_width = lw->label.lbm_height = 0;
    }
    compute_bitmap_offsets (lw);
}



/* ARGSUSED */
static void Initialize(request, new, args, num_args)
    Widget request, new;
    ArgList args;
    Cardinal *num_args;
{
    LabelWidget lw = (LabelWidget) new;

    if (lw->label.label == NULL) 
        lw->label.label = XtNewString(lw->core.name);
    else {
        lw->label.label = XtNewString(lw->label.label);
    }

    GetnormalGC(lw);
    GetgrayGC(lw);

    SetTextWidthAndHeight(lw);

    if (lw->core.height == 0)
        lw->core.height = lw->label.label_height +
			    2 * lw->label.internal_height +
			    2 * lw->threeD.shadow_width;

    set_bitmap_info (lw);		/* need core.height */

    if (lw->core.width == 0)		/* need label.lbm_width */
        lw->core.width = (lw->label.label_width +
			    2 * lw->label.internal_width +
			    2 * lw->threeD.shadow_width +
			    LEFT_OFFSET(lw));

    lw->label.label_x = lw->label.label_y = 0;
    (*XtClass(new)->core_class.resize) ((Widget)lw);

} /* Initialize */

/*
 * Repaint the widget window
 */

/* ARGSUSED */
static void Redisplay(gw, event, region)
    Widget gw;
    XEvent *event;
    Region region;
{
    extern WidgetClass commandWidgetClass;
    LabelWidget w = (LabelWidget) gw;
    LabelWidgetClass lwclass = (LabelWidgetClass) XtClass (gw);
    GC gc;

    /* 
     * Don't draw shadows if command is going to redraw them.
     * The shadow draw method is region aware, but since 99% of
     * all labels don't have shadows, we'll check for a shadow
     * before we incur the function call overhead.
     */
    if (!XtIsSubclass (gw, commandWidgetClass) && w->threeD.shadow_width > 0)
	(*lwclass->threeD_class.shadowdraw) (gw, event, region, True);

    /*
     * now we'll see if we need to draw the rest of the label
     */
    if (region != NULL) {
	int x = w->label.label_x;
	unsigned int width = w->label.label_width;
	if (w->label.lbm_width) {
	    if (w->label.label_x > (x = w->label.internal_width))
		width += w->label.label_x - x;
	}
	if (XRectInRegion(region, x, w->label.label_y,
			 width, w->label.label_height) == RectangleOut){
	    return;
	}
    }

    gc = XtIsSensitive(gw) ? w->label.normal_GC : w->label.gray_GC;
#ifdef notdef
    if (region != NULL)
	XSetRegion(XtDisplay(gw), gc, region);
#endif /*notdef*/

    if (w->label.pixmap == None) {
	int len = w->label.label_len;
	char *label = w->label.label;
	Position y = w->label.label_y + w->label.font->max_bounds.ascent;
        Position ksy = w->label.label_y;

	/* display left bitmap */
	if (w->label.left_bitmap && w->label.lbm_width != 0) {
	    XCopyPlane (XtDisplay(gw), w->label.left_bitmap, XtWindow(gw), gc,
		       0, 0, w->label.lbm_width, w->label.lbm_height,
		       (int) w->label.internal_width
			+ w->threeD.shadow_width,
		       (int) w->label.internal_height 
			+ w->threeD.shadow_width
			+ w->label.lbm_y, 
		       (unsigned long) 1L);
	}

        if ( w->simple.international == True ) {

	    XFontSetExtents *ext = XExtentsOfFontSet(w->label.fontset);

	    ksy += abs(ext->max_ink_extent.y);

            if (len == MULTI_LINE_LABEL) {
	        char *nl;
	        while ((nl = index(label, '\n')) != NULL) {
	            XmbDrawString(XtDisplay(w), XtWindow(w), w->label.fontset, gc,
	  		        w->label.label_x, ksy, label, (int)(nl - label));
	            ksy += ext->max_ink_extent.height;
	            label = nl + 1;
	        }
	        len = strlen(label);
            }
            if (len)
	        XmbDrawString(XtDisplay(w), XtWindow(w), w->label.fontset, gc,
			      w->label.label_x, ksy, label, len);

        } else { /*international false, so use R5 routine */

	    if (len == MULTI_LINE_LABEL) {
	        char *nl;
	        while ((nl = index(label, '\n')) != NULL) {
		    if (w->label.encoding)
		        XDrawString16(XtDisplay(gw), XtWindow(gw), gc,
				 		w->label.label_x, y,
				 		(TXT16*)label, (int)(nl - label)/2);
		    else
		        XDrawString(XtDisplay(gw), XtWindow(gw), gc,
			       		w->label.label_x, y, label, (int)(nl - label));
		    y += w->label.font->max_bounds.ascent + 
		                        w->label.font->max_bounds.descent;
		    label = nl + 1;
	        }
	        len = strlen(label);
	    }
	    if (len) {
	        if (w->label.encoding)
		    XDrawString16(XtDisplay(gw), XtWindow(gw), gc,
			     w->label.label_x, y, (TXT16*)label, len/2);
	        else
		    XDrawString(XtDisplay(gw), XtWindow(gw), gc,
			   w->label.label_x, y, label, len);
	    }

        } /*endif international*/

    } else if (w->label.label_len == 1) { /* depth */
	XCopyPlane(XtDisplay(gw), w->label.pixmap, XtWindow(gw), gc,
				0, 0, w->label.label_width, w->label.label_height,
				w->label.label_x, w->label.label_y, 1L);
    } else {
	XCopyArea(XtDisplay(gw), w->label.pixmap, XtWindow(gw), gc,
				0, 0, w->label.label_width, w->label.label_height,
				w->label.label_x, w->label.label_y);
    }

#ifdef notdef
    if (region != NULL)
	XSetClipMask(XtDisplay(gw), gc, (Pixmap)None);
#endif /* notdef */
}

static void _Reposition(lw, width, height, dx, dy)
    LabelWidget lw;
    Dimension width, height;
    Position *dx, *dy;
{
    Position newPos;
    Position leftedge = lw->label.internal_width + LEFT_OFFSET(lw) +
			lw->threeD.shadow_width;

    switch (lw->label.justify) {

	case XtJustifyLeft   :
	    newPos = leftedge;
	    break;

	case XtJustifyRight  :
	    newPos = width -
		(lw->label.label_width 
			+ lw->label.internal_width 
			+ lw->threeD.shadow_width);
	    break;

	case XtJustifyCenter :
	default:
	    newPos = (int)(width - lw->label.label_width) / 2;
	    break;
    }
    if (newPos < (Position)leftedge)
	newPos = leftedge;
    *dx = newPos - lw->label.label_x;
    lw->label.label_x = newPos;
    *dy = (newPos = (int)(height - lw->label.label_height) / 2)
	  - lw->label.label_y;
    lw->label.label_y = newPos;
    return;
}

static void Resize(w)
    Widget w;
{
    LabelWidget lw = (LabelWidget)w;
    Position dx, dy;

    _Reposition(lw, w->core.width, w->core.height, &dx, &dy);
    compute_bitmap_offsets (lw);
}

/*
 * Set specified arguments into widget
 */

#define PIXMAP 0
#define WIDTH 1
#define HEIGHT 2
#define NUM_CHECKS 3

static Boolean SetValues(current, request, new, args, num_args)
    Widget current, request, new;
    ArgList args;
    Cardinal *num_args;
{
    LabelWidget curlw = (LabelWidget) current;
    LabelWidget reqlw = (LabelWidget) request;
    LabelWidget newlw = (LabelWidget) new;
    int i;
    Boolean was_resized = False, redisplay = False, checks[NUM_CHECKS];

    for (i = 0; i < NUM_CHECKS; i++)
	checks[i] = FALSE;

    for (i = 0; i < *num_args; i++) {
	if (streq(XtNbitmap, args[i].name))
	    checks[PIXMAP] = TRUE;
	if (streq(XtNwidth, args[i].name))
	    checks[WIDTH] = TRUE;
	if (streq(XtNheight, args[i].name))
	    checks[HEIGHT] = TRUE;
    }

    if (newlw->label.label == NULL) {
	newlw->label.label = newlw->core.name;
    }

    /*
     * resize on bitmap change
     */
    if (curlw->label.left_bitmap != newlw->label.left_bitmap) {
	was_resized = True;
    }

    if (curlw->label.encoding != newlw->label.encoding)
	was_resized = True;

    if ( (curlw->label.fontset != newlw->label.fontset) &&
					 curlw->simple.international ){
	was_resized = True;
    }
    if (curlw->label.label != newlw->label.label) {
        if (curlw->label.label != curlw->core.name)
	    XtFree( (char *)curlw->label.label );

	if (newlw->label.label != newlw->core.name) {
	    newlw->label.label = XtNewString( newlw->label.label );
	}
	was_resized = True;
    }

    if (was_resized || (curlw->label.font != newlw->label.font) ||
	(curlw->label.justify != newlw->label.justify) || checks[PIXMAP]) {

	SetTextWidthAndHeight(newlw);
	was_resized = True;
    }

    /* recalculate the window size if something has changed. */
    if (newlw->label.resize && was_resized) {
	if ((curlw->core.height == reqlw->core.height) && !checks[HEIGHT])
	    newlw->core.height = (newlw->label.label_height +
				    2 * newlw->label.internal_height +
				    2 * newlw->threeD.shadow_width);

	set_bitmap_info (newlw);

	if ((curlw->core.width == reqlw->core.width) && !checks[WIDTH])
	    newlw->core.width = (newlw->label.label_width +
				    LEFT_OFFSET(newlw) +
				    2 * newlw->label.internal_width +
				    2 * newlw->threeD.shadow_width);
    }

    if (curlw->label.foreground		!= newlw->label.foreground
	|| curlw->core.background_pixel != newlw->core.background_pixel
	|| curlw->label.font->fid	!= newlw->label.font->fid	  ) {

        /* The Fontset is not in the GC - don't make a new GC if FS changes! */

	XtReleaseGC(new, curlw->label.normal_GC);
	XtReleaseGC(new, curlw->label.gray_GC);
	XmuReleaseStippledPixmap( XtScreen(current), curlw->label.stipple );
	GetnormalGC(newlw);
	GetgrayGC(newlw);
	redisplay = True;
    }

    if ((curlw->label.internal_width != newlw->label.internal_width)
        || (curlw->label.internal_height != newlw->label.internal_height)
	|| was_resized) {
	/* Resize() will be called if geometry changes succeed */
	Position dx, dy;
	_Reposition(newlw, curlw->core.width, curlw->core.height, &dx, &dy);
    }

    return was_resized || redisplay ||
	   XtIsSensitive(current) != XtIsSensitive(new);
}

static void Destroy(w)
    Widget w;
{
    LabelWidget lw = (LabelWidget)w;

    if ( lw->label.label != lw->core.name )
	XtFree( lw->label.label );
    XtReleaseGC( w, lw->label.normal_GC );
    XtReleaseGC( w, lw->label.gray_GC);
    XmuReleaseStippledPixmap( XtScreen(w), lw->label.stipple );
}


static XtGeometryResult QueryGeometry(w, intended, preferred)
    Widget w;
    XtWidgetGeometry *intended, *preferred;
{
    LabelWidget lw = (LabelWidget)w;

    preferred->request_mode = CWWidth | CWHeight;
    preferred->width = (lw->label.label_width + 
			    2 * lw->label.internal_width +
			    2 * lw->threeD.shadow_width +
			    LEFT_OFFSET(lw));
    preferred->height = lw->label.label_height + 
			    2 * lw->label.internal_height +
			    2 * lw->threeD.shadow_width;
    if (  ((intended->request_mode & (CWWidth | CWHeight))
	   	== (CWWidth | CWHeight)) &&
	  intended->width == preferred->width &&
	  intended->height == preferred->height)
	return XtGeometryYes;
    else if (preferred->width == w->core.width &&
	     preferred->height == w->core.height)
	return XtGeometryNo;
    else
	return XtGeometryAlmost;
}
