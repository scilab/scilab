/*
 *	$XConsortium: scrollbar.c,v 1.41 91/05/22 15:20:07 gildea Exp $
 */

/*
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Digital Equipment
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 *
 *
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#include "x_ptyxP.h"		/* gets Xt headers, too */

#include <stdio.h>
#include <ctype.h>
#ifdef __STDC__
#include <stdlib.h>
#else 
#include <malloc.h>
#endif

#include <X11/Xatom.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw3d/Scrollbar.h>

#include "x_data.h"
#include "x_error.h"
#include "x_menu.h"

#include "../machine.h"
#include "All-extern-x.h"

/* Event handlers */

static void ScrollTextTo();
static void ScrollTextUpDownBy();


/* resize the text window for a terminal screen, modifying the
 * appropriate WM_SIZE_HINTS and taking advantage of bit gravity.
 */

static void ResizeScreen(xw, min_width, min_height )
	register XtermWidget xw;
	int min_width, min_height;
{
	register TScreen *screen = &xw->screen;
#ifndef nothack
	XSizeHints sizehints;
	long supp;
#endif
	XtGeometryResult geomreqresult;
	Dimension reqWidth, reqHeight, repWidth, repHeight;

	/*
	 * I'm going to try to explain, as I understand it, why we
	 * have to do XGetWMNormalHints and XSetWMNormalHints here,
	 * although I can't guarantee that I've got it right.
	 *
	 * In a correctly written toolkit program, the Shell widget
	 * parses the user supplied geometry argument.  However,
	 * because of the way xterm does things, the VT100 widget does
	 * the parsing of the geometry option, not the Shell widget.
	 * The result of this is that the Shell widget doesn't set the
	 * correct window manager hints, and doesn't know that the
	 * user has specified a geometry.
	 *
	 * The XtVaSetValues call below tells the Shell widget to
	 * change its hints.  However, since it's confused about the
	 * hints to begin with, it doesn't get them all right when it
	 * does the SetValues -- it undoes some of what the VT100
	 * widget did when it originally set the hints.
	 *
	 * To fix this, we do the following:
	 *
	 * 1. Get the sizehints directly from the window, going around
	 *    the (confused) shell widget.
	 * 2. Call XtVaSetValues to let the shell widget know which
	 *    hints have changed.  Note that this may not even be
	 *    necessary, since we're going to right ahead after that
	 *    and set the hints ourselves, but it's good to put it
	 *    here anyway, so that when we finally do fix the code so
	 *    that the Shell does the right thing with hints, we
	 *    already have the XtVaSetValues in place.
	 * 3. We set the sizehints directly, this fixing up whatever
	 *    damage was done by the Shell widget during the
	 *    XtVaSetValues.
	 *
	 * Gross, huh?
	 *
	 * The correct fix is to redo VTRealize, VTInitialize and
	 * VTSetValues so that font processing happens early enough to
	 * give back responsibility for the size hints to the Shell.
	 *
	 * Someday, we hope to have time to do this.  Someday, we hope
	 * to have time to completely rewrite xterm.
	 */

#ifndef nothack
	/*
	 * NOTE: If you change the way any of the hints are calculated
	 * below, make sure you change the calculation both in the
	 * sizehints assignments and in the XtVaSetValues.
	 */

	if (! XGetWMNormalHints(screen->display, XtWindow(XtParent(xw)),
				&sizehints, &supp))
	     sizehints.flags = 0;
	sizehints.base_width = min_width;
	sizehints.base_height = min_height;
	sizehints.width_inc = FontWidth(screen);
	sizehints.height_inc = FontHeight(screen);
	sizehints.min_width = sizehints.base_width + sizehints.width_inc;
	sizehints.min_height = sizehints.base_height + sizehints.height_inc;
	sizehints.flags |= (PBaseSize|PMinSize|PResizeInc);
	/* These are obsolete, but old clients may use them */
	sizehints.width = (screen->max_col + 1) * FontWidth(screen)
	     + min_width;
	sizehints.height = (screen->max_row + 1) * FontHeight(screen)
	     + min_height;
#endif
	
	/*
	 * Note: width and height are not set here because they are 
	 * obsolete. 						
	 */
	XtVaSetValues(XtParent(xw),
		      XtNbaseWidth, min_width,
		      XtNbaseHeight, min_height,
		      XtNwidthInc, FontWidth(screen),
		      XtNheightInc, FontHeight(screen),
		      XtNminWidth, min_width + FontWidth(screen),
		      XtNminHeight, min_height + FontHeight(screen),
		      NULL);

	reqWidth = (screen->max_col + 1) * FontWidth(screen) + min_width;
	reqHeight = FontHeight(screen) * (screen->max_row + 1) + min_height;
	geomreqresult = XtMakeResizeRequest ((Widget)xw, reqWidth, reqHeight,
					     &repWidth, &repHeight);

	if (geomreqresult == XtGeometryAlmost) {
	     geomreqresult = XtMakeResizeRequest ((Widget)xw, repWidth,
						  repHeight, (Dimension *) NULL,(Dimension*) NULL);
	}

#ifndef nothack
	XSetWMNormalHints(screen->display, XtWindow(XtParent(xw)), &sizehints);
#endif
}

void DoResizeScreen (xw)
    register XtermWidget xw;
{
    int border = 2 * xw->screen.border;
    ResizeScreen (xw, border + xw->screen.scrollbar, border);
}


static Widget CreateScrollBar(xw, x, y, height)
	XtermWidget xw;
	int x, y, height;
{
	Widget scrollWidget;

	static Arg argList[] = {
	   {XtNx,		(XtArgVal) 0},
	   {XtNy,		(XtArgVal) 0},
	   {XtNheight,		(XtArgVal) 0},
	   {XtNreverseVideo,	(XtArgVal) 0},
	   {XtNorientation,	(XtArgVal) XtorientVertical},
	   {XtNborderWidth,	(XtArgVal) 1},
	};   

	argList[0].value = (XtArgVal) x;
	argList[1].value = (XtArgVal) y;
	argList[2].value = (XtArgVal) height;
	argList[3].value = (XtArgVal) xw->misc.re_verse;

	scrollWidget = XtCreateWidget("scrollbar", scrollbarWidgetClass, 
	  (Widget)xw, argList, XtNumber(argList));
        XtAddCallback (scrollWidget, XtNscrollProc, ScrollTextUpDownBy,(XtPointer) 0);
        XtAddCallback (scrollWidget, XtNjumpProc, ScrollTextTo, (XtPointer)0);
	return (scrollWidget);
}

static void RealizeScrollBar (sbw, screen)
    Widget sbw;
    TScreen *screen;
{
    XtRealizeWidget (sbw);
}


void ScrollBarReverseVideo(scrollWidget)
	register Widget scrollWidget;
{
	Arg args[4];
	Cardinal nargs = XtNumber(args);
	unsigned long bg, fg, bdr;
	Pixmap bdpix;

	XtSetArg(args[0], XtNbackground, &bg);
	XtSetArg(args[1], XtNforeground, &fg);
	XtSetArg(args[2], XtNborderColor, &bdr);
	XtSetArg(args[3], XtNborderPixmap, &bdpix);
	XtGetValues (scrollWidget, args, nargs);
	args[0].value = (XtArgVal) fg;
	args[1].value = (XtArgVal) bg;
	nargs--;				/* don't set border_pixmap */
	if (bdpix == XtUnspecifiedPixmap) {	/* if not pixmap then pixel */
	    args[2].value = args[1].value;	/* set border to new fg */
	} else {				/* ignore since pixmap */
	    nargs--;				/* don't set border pixel */
	}
	XtSetValues (scrollWidget, args, nargs);
}



void ScrollBarDrawThumb(scrollWidget)
	register Widget scrollWidget;
{
	register TScreen *screen = &term->screen;
	register int thumbTop, thumbHeight, totalHeight;
	
	thumbTop    = screen->topline + screen->savedlines;
	thumbHeight = screen->max_row + 1;
	totalHeight = thumbHeight + screen->savedlines;

	XawScrollbarSetThumb(scrollWidget,
	 ((float)thumbTop) / totalHeight,
	 ((float)thumbHeight) / totalHeight);
	
}

void ResizeScrollBar(scrollWidget, x, y, height)
	register Widget scrollWidget;
	int x, y;
	unsigned height;
{
	XtConfigureWidget(scrollWidget, x, y, scrollWidget->core.width,
	    height, scrollWidget->core.border_width);
	ScrollBarDrawThumb(scrollWidget);
}

void WindowScroll(screen, top)
	register TScreen *screen;
	int top;
{
	register int i, lines;
	register int scrolltop, scrollheight, refreshtop;
	register int x = 0;

	if (top < -screen->savedlines)
		top = -screen->savedlines;
	else if (top > 0)
		top = 0;
	if((i = screen->topline - top) == 0) {
		ScrollBarDrawThumb(screen->scrollWidget);
		return;
	}

	ScrollSelection(screen, i);

	if(screen->cursor_state)
		HideCursor();
	lines = i > 0 ? i : -i;
	if(lines > screen->max_row + 1)
		lines = screen->max_row + 1;
	scrollheight = screen->max_row - lines + 1;
	if(i > 0)
		refreshtop = scrolltop = 0;
	else {
		scrolltop = lines;
		refreshtop = scrollheight;
	}
	x = screen->scrollbar +	screen->border;
	scrolling_copy_area(screen, scrolltop, scrollheight, -i);
	screen->topline = top;
	XClearArea(
	    screen->display,
	    TextWindow(screen), 
	    (int) x,
	    (int) refreshtop * FontHeight(screen) + screen->border, 
	    (unsigned) Width(screen),
	    (unsigned) lines * FontHeight(screen),
	    FALSE);
	ScrnRefresh(screen, refreshtop, 0, lines, screen->max_col + 1, False);

	ScrollBarDrawThumb(screen->scrollWidget);
}


void ScrollBarOn (xw, init, doalloc)
    XtermWidget xw;
    int init, doalloc;
{
	register TScreen *screen = &xw->screen;
	register int border = 2 * screen->border;
	register int i;

	if(screen->scrollbar)
		return;

	if (init) {			/* then create it only */
	    if (screen->scrollWidget) return;

	    /* make it a dummy size and resize later */
	    if ((screen->scrollWidget = CreateScrollBar (xw, -1, - 1, 5))
		== NULL) {
		Bell();
		return;
	    }

	    return;

	}

	if (!screen->scrollWidget) {
	    Bell ();
	    Bell ();
	    return;
	}

	if (doalloc && screen->allbuf) {
	    if((screen->allbuf =
		(ScrnBuf) realloc((char *) screen->buf,
				  (unsigned) 2*(screen->max_row + 2 +
						screen->savelines) *
				  sizeof(char *)))
	       == NULL)
	      Error (ERROR_SBRALLOC);
	    screen->buf = &screen->allbuf[2 * screen->savelines];
	    bcopy ((char *)screen->allbuf, (char *)screen->buf,
		   2 * (screen->max_row + 2) * sizeof (char *));
	    for(i = 2 * screen->savelines - 1 ; i >= 0 ; i--)
	      if((screen->allbuf[i] = (Char *) 
		  calloc((unsigned) screen->max_col + 1, sizeof(char))) ==
		 NULL)
		Error (ERROR_SBRALLOC2);
	}

	ResizeScrollBar (screen->scrollWidget, -1, -1, 
			(unsigned) Height (screen) + border);
	RealizeScrollBar (screen->scrollWidget, screen);
	screen->scrollbar = screen->scrollWidget->core.width +
	     screen->scrollWidget->core.border_width;

	ScrollBarDrawThumb(screen->scrollWidget);
	DoResizeScreen (xw);
	XtMapWidget(screen->scrollWidget);
	update_scrollbar ();
	if (screen->buf) {
	    XClearWindow (screen->display, XtWindow (term));
	    Redraw ();
	}
}

void ScrollBarOff(screen)
	register TScreen *screen;
{
	if(!screen->scrollbar)
		return;
	XtUnmapWidget(screen->scrollWidget);
	screen->scrollbar = 0;
	DoResizeScreen (term);
	update_scrollbar ();
	if (screen->buf) {
	    XClearWindow (screen->display, XtWindow (term));
	    Redraw ();
	}
}

/*ARGSUSED*/
static void ScrollTextTo(scrollbarWidget, closure, topPercent)
	Widget scrollbarWidget;
	caddr_t closure;
	float *topPercent;
{
	register TScreen *screen = &term->screen;
	int thumbTop;	/* relative to first saved line */
	int newTopLine;

/*
   screen->savedlines : Number of offscreen text lines,
   screen->maxrow + 1 : Number of onscreen  text lines,
   screen->topline    : -Number of lines above the last screen->max_row+1 lines
*/

	thumbTop = *topPercent * (screen->savedlines + screen->max_row+1);
	newTopLine = thumbTop - screen->savedlines;
	WindowScroll(screen, newTopLine);
}

/*ARGSUSED*/
static void ScrollTextUpDownBy(scrollbarWidget, closure, pixels)
	Widget scrollbarWidget;
	Opaque closure;
	int pixels;
{
	register TScreen *screen = &term->screen;
	register int rowOnScreen, newTopLine;

	rowOnScreen = pixels / FontHeight(screen);
	if (rowOnScreen == 0) {
		if (pixels < 0)
			rowOnScreen = -1;
		else if (pixels > 0)
			rowOnScreen = 1;
	}
	newTopLine = screen->topline + rowOnScreen;
	WindowScroll(screen, newTopLine);
}


/*
 * assume that b is lower case and allow plural
 */
static int specialcmplowerwiths (a, b)
    char *a, *b;
{
    register char ca, cb;

    if (!a || !b) return 0;

    while (1) {
	ca = *a;
	cb = *b;
	if (isascii(ca) && isupper(ca)) {		/* lowercasify */
#ifdef _tolower
	    ca = _tolower (ca);
#else
	    ca = tolower (ca);
#endif
	}
	if (ca != cb || ca == '\0') break;  /* if not eq else both nul */
	a++, b++;
    }
    if (cb == '\0' && (ca == '\0' || (ca == 's' && a[1] == '\0')))
      return 1;

    return 0;
}

static int params_to_pixels (screen, params, n)
    TScreen *screen;
    String *params;
    int n;
{
    register int mult = 1;
    register char *s;

    switch (n > 2 ? 2 : n) {
      case 2:
	s = params[1];
	if (specialcmplowerwiths (s, "page")) {
	    mult = (screen->max_row + 1) * FontHeight(screen);
	} else if (specialcmplowerwiths (s, "halfpage")) {
	    mult = ((screen->max_row + 1) * FontHeight(screen)) >> 1;
	} else if (specialcmplowerwiths (s, "pixel")) {
	    mult = 1;
	} /* else assume that it is Line */
	mult *= atoi (params[0]);
	break;
      case 1:
	mult = atoi (params[0]) * FontHeight(screen);	/* lines */
	break;
      default:
	mult = screen->scrolllines * FontHeight(screen);
	break;
    }

    return mult;
}


/*ARGSUSED*/
void HandleScrollForward (gw, event, params, nparams)
    Widget gw;
    XEvent *event;
    String *params;
    Cardinal *nparams;
{
    XtermWidget w = (XtermWidget) gw;
    register TScreen *screen = &w->screen;

    ScrollTextUpDownBy (gw, (Opaque) NULL,
			params_to_pixels (screen, params, (int) *nparams));
    return;
}


/*ARGSUSED*/
void HandleScrollBack (gw, event, params, nparams)
    Widget gw;
    XEvent *event;
    String *params;
    Cardinal *nparams;
{
    XtermWidget w = (XtermWidget) gw;
    register TScreen *screen = &w->screen;

    ScrollTextUpDownBy (gw, (Opaque) NULL,
			-params_to_pixels (screen, params, (int) *nparams));
    return;
}








