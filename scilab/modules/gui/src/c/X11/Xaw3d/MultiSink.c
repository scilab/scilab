/* $XConsortium: MultiSink.c,v 1.6 95/01/23 18:34:46 kaleb Exp $ */

/*
 * Copyright 1991 by OMRON Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OMRON not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  OMRON makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * OMRON DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OMRON BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *      Author: Li Yuhong	 OMRON Corporation
 */

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

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/MultiSinkP.h>
#include <X11/Xaw3d/MultiSrcP.h>
#include <X11/Xaw3d/TextP.h>
#include "XawI18n.h"
#include <stdio.h>
#include <ctype.h>

#ifdef GETLASTPOS
#undef GETLASTPOS		/* We will use our own GETLASTPOS. */
#endif

#define GETLASTPOS XawTextSourceScan(source, (XawTextPosition) 0, XawstAll, XawsdRight, 1, TRUE)

static void Initialize(), Destroy();
static Boolean SetValues();
static int MaxLines(), MaxHeight();
static void SetTabs();

static void DisplayText(), InsertCursor(), FindPosition();
static void FindDistance(), Resolve(), GetCursorBounds();

#define offset(field) XtOffsetOf(MultiSinkRec, multi_sink.field)

static XtResource resources[] = {
    {XtNfontSet, XtCFontSet, XtRFontSet, sizeof (XFontSet),
	offset(fontset), XtRString, XtDefaultFontSet},
    {XtNecho, XtCOutput, XtRBoolean, sizeof(Boolean),
	offset(echo), XtRImmediate, (XtPointer) True},
    {XtNdisplayNonprinting, XtCOutput, XtRBoolean, sizeof(Boolean),
	offset(display_nonprinting), XtRImmediate, (XtPointer) True},
};
#undef offset

#define SuperClass		(&textSinkClassRec)
MultiSinkClassRec multiSinkClassRec = {
  { /* core_class fields */	
    /* superclass	  	*/	(WidgetClass) SuperClass,
    /* class_name	  	*/	"MultiSink",
    /* widget_size	  	*/	sizeof(MultiSinkRec),
    /* class_initialize   	*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* obj1		  	*/	NULL,
    /* obj2		  	*/	NULL,
    /* obj3		  	*/	0,
    /* resources	  	*/	resources,
    /* num_resources	  	*/	XtNumber(resources),
    /* xrm_class	  	*/	NULLQUARK,
    /* obj4		  	*/	FALSE,
    /* obj5		  	*/	FALSE,
    /* obj6			*/	FALSE,
    /* obj7		  	*/	FALSE,
    /* destroy		  	*/	Destroy,
    /* obj8		  	*/	NULL,
    /* obj9		  	*/	NULL,
    /* set_values	  	*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* obj10			*/	NULL,
    /* get_values_hook		*/	NULL,
    /* obj11		 	*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private   	*/	NULL,
    /* obj12		   	*/	NULL,
    /* obj13			*/	NULL,
    /* obj14			*/	NULL,
    /* extension		*/	NULL
  },
  { /* text_sink_class fields */
    /* DisplayText              */      DisplayText,
    /* InsertCursor             */      InsertCursor,
    /* ClearToBackground        */      XtInheritClearToBackground,
    /* FindPosition             */      FindPosition,
    /* FindDistance             */      FindDistance,
    /* Resolve                  */      Resolve,
    /* MaxLines                 */      MaxLines,
    /* MaxHeight                */      MaxHeight,
    /* SetTabs                  */      SetTabs,
    /* GetCursorBounds          */      GetCursorBounds
  },
  { /* multi_sink_class fields */
    /* unused			*/	0
  }
};

WidgetClass multiSinkObjectClass = (WidgetClass)&multiSinkClassRec;

/* Utilities */

static int 
#if NeedFunctionPrototypes
CharWidth (
    Widget w,
    int x,
    wchar_t c)
#else
CharWidth (w, x, c)
    Widget w;
    int x;
    wchar_t c;
#endif
{
    int    i, width;
    MultiSinkObject sink = (MultiSinkObject) w;
    XFontSet fontset = sink->multi_sink.fontset;
    Position *tab;

    if ( c == _Xaw_atowc(XawLF) ) return(0);

    if (c == _Xaw_atowc(XawTAB)) {
	/* Adjust for Left Margin. */
	x -= ((TextWidget) XtParent(w))->text.margin.left;

	if (x >= (int)XtParent(w)->core.width) return 0;
	for (i = 0, tab = sink->text_sink.tabs ; 
	     i < sink->text_sink.tab_count ; i++, tab++) {
	    if (x < *tab) {
		if (*tab < (int)XtParent(w)->core.width)
		    return *tab - x;
		else
		    return 0;
	    }
	}
	return 0;
    }

    if (XwcTextEscapement (fontset, &c, 1) == 0)
	if (sink->multi_sink.display_nonprinting)
	    c = _Xaw_atowc('@');
	else {
	    c = _Xaw_atowc(XawSP);
	}

    /*
     * if more efficiency(suppose one column is one ASCII char)

    width = XwcGetColumn(fontset->font_charset, fontset->num_of_fonts, c) *
            fontset->font_struct_list[0]->min_bounds.width;
     *
     * WARNING: Very Slower!!!
     *
     * Li Yuhong.
     */

    width = XwcTextEscapement(fontset, &c, 1);

    return width;
}

/*	Function Name: PaintText
 *	Description: Actually paints the text into the windoe.
 *	Arguments: w - the text widget.
 *                 gc - gc to paint text with.
 *                 x, y - location to paint the text.
 *                 buf, len - buffer and length of text to paint.
 *	Returns: the width of the text painted, or 0.
 *
 * NOTE:  If this string attempts to paint past the end of the window
 *        then this function will return zero.
 */

static Dimension
PaintText(w, gc, x, y, buf, len)
    Widget w;
    GC gc;
    Position x, y;
    wchar_t* buf;
    int len;
{
    MultiSinkObject sink = (MultiSinkObject) w;
    TextWidget ctx = (TextWidget) XtParent(w);

    XFontSet fontset = sink->multi_sink.fontset;
    Position max_x;
    Dimension width = XwcTextEscapement(fontset, buf, len);
    XFontSetExtents *ext = XExtentsOfFontSet(fontset);
    max_x = (Position) ctx->core.width;

    if ( ((int) width) <= -x)	           /* Don't draw if we can't see it. */
      return(width);

    XwcDrawImageString(XtDisplay(ctx), XtWindow(ctx), fontset, gc,
                     (int) x, (int) y, buf, len);
    if ( (((Position) width + x) > max_x) && (ctx->text.margin.right != 0) ) {
	x = ctx->core.width - ctx->text.margin.right;
	width = ctx->text.margin.right;
	XFillRectangle(XtDisplay((Widget) ctx), XtWindow( (Widget) ctx),
		       sink->multi_sink.normgc, (int) x,
                       (int) y - abs(ext->max_logical_extent.y),
                       (unsigned int) width,
                       (unsigned int) ext->max_logical_extent.height);
	return(0);
    }
    return(width);
}

/* Sink Object Functions */

/*
 * This function does not know about drawing more than one line of text.
 */
 
static void 
DisplayText(w, x, y, pos1, pos2, highlight)
    Widget w;
    Position x, y;
    Boolean highlight;
    XawTextPosition pos1, pos2;
{
    MultiSinkObject sink = (MultiSinkObject) w;
    Widget source = XawTextGetSource(XtParent(w));
    wchar_t buf[BUFSIZ];
    XFontSetExtents *ext = XExtentsOfFontSet(sink->multi_sink.fontset);

    int j, k;
    XawTextBlock blk;
    GC gc = highlight ? sink->multi_sink.invgc : sink->multi_sink.normgc;
    GC invgc = highlight ? sink->multi_sink.normgc : sink->multi_sink.invgc;

    if (!sink->multi_sink.echo) return;

    y += abs(ext->max_logical_extent.y);
    for ( j = 0 ; pos1 < pos2 ; ) {
	pos1 = XawTextSourceRead(source, pos1, &blk, (int) pos2 - pos1);
	for (k = 0; k < blk.length; k++) {
	    if (j >= BUFSIZ) {	/* buffer full, dump the text. */
	        x += PaintText(w, gc, x, y, buf, j);
		j = 0;
	    }
	    buf[j] = ((wchar_t *)blk.ptr)[k];
	    if (buf[j] == _Xaw_atowc(XawLF))
	        continue;

	    else if (buf[j] == _Xaw_atowc(XawTAB)) {
	        Position temp = 0;
		Dimension width;

	        if ((j != 0) && ((temp = PaintText(w, gc, x, y, buf, j)) == 0))
		  return;

	        x += temp;
                width = CharWidth(w, x, _Xaw_atowc(XawTAB));
		XFillRectangle(XtDisplayOfObject(w), XtWindowOfObject(w),
			       invgc, (int) x,
                               (int) y - abs(ext->max_logical_extent.y),
                               (unsigned int)width,
                               (unsigned int)ext->max_logical_extent.height);
                x += width;
                j = -1;
            }
            else if (XwcTextEscapement (sink->multi_sink.fontset, &buf[j], 1) == 0) {
                if (sink->multi_sink.display_nonprinting)
                    buf[j] = _Xaw_atowc('@');
                else
                    buf[j] = _Xaw_atowc(' ');
            }
	    j++;
	}
    }
    if (j > 0)
        (void) PaintText(w, gc, x, y, buf, j);
}

#define insertCursor_width 6
#define insertCursor_height 3
static char insertCursor_bits[] = {0x0c, 0x1e, 0x33};

static Pixmap
CreateInsertCursor(s)
    Screen *s;
{
    return (XCreateBitmapFromData (DisplayOfScreen(s), RootWindowOfScreen(s),
		  insertCursor_bits, insertCursor_width, insertCursor_height));
}

/*	Function Name: GetCursorBounds
 *	Description: Returns the size and location of the cursor.
 *	Arguments: w - the text object.
 * RETURNED        rect - an X rectangle to return the cursor bounds in.
 *	Returns: none.
 */

static void
GetCursorBounds(w, rect)
    Widget w;
    XRectangle * rect;
{
    MultiSinkObject sink = (MultiSinkObject) w;

    rect->width = (unsigned short) insertCursor_width;
    rect->height = (unsigned short) insertCursor_height;
    rect->x = sink->multi_sink.cursor_x - (short) (rect->width / 2);
    rect->y = sink->multi_sink.cursor_y - (short) rect->height;
}

/*
 * The following procedure manages the "insert" cursor.
 */

static void
InsertCursor (w, x, y, state)
    Widget w;
    Position x, y;
    XawTextInsertState state;
{
    MultiSinkObject sink = (MultiSinkObject) w;
    Widget text_widget = XtParent(w);
    XRectangle rect;

    sink->multi_sink.cursor_x = x;
    sink->multi_sink.cursor_y = y;

    GetCursorBounds(w, &rect);
    if (state != sink->multi_sink.laststate && XtIsRealized(text_widget)) 
        XCopyPlane(XtDisplay(text_widget),
		   sink->multi_sink.insertCursorOn,
		   XtWindow(text_widget), sink->multi_sink.xorgc,
		   0, 0, (unsigned int) rect.width, (unsigned int) rect.height,
		   (int) rect.x, (int) rect.y, 1);
    sink->multi_sink.laststate = state;
}

/*
 * Given two positions, find the distance between them.
 */

static void
FindDistance (w, fromPos, fromx, toPos, resWidth, resPos, resHeight)
    Widget w;
    XawTextPosition fromPos;	/* First position. */
    int fromx;			/* Horizontal location of first position. */
    XawTextPosition toPos;	/* Second position. */
    int* resWidth;		/* Distance between fromPos and resPos. */
    XawTextPosition* resPos;	/* Actual second position used. */
    int* resHeight;		/* Height required. */
{
    MultiSinkObject sink = (MultiSinkObject) w;
    Widget source = XawTextGetSource(XtParent(w));

    XawTextPosition index, lastPos;
    wchar_t c;
    XFontSetExtents *ext = XExtentsOfFontSet(sink->multi_sink.fontset);
    XawTextBlock blk;

    /* we may not need this */
    lastPos = GETLASTPOS;
    XawTextSourceRead(source, fromPos, &blk, (int) toPos - fromPos);
    *resWidth = 0;
    for (index = fromPos; index != toPos && index < lastPos; index++) {
	if (index - blk.firstPos >= blk.length)
	    XawTextSourceRead(source, index, &blk, (int) toPos - fromPos);
        c = ((wchar_t *)blk.ptr)[index - blk.firstPos];
	*resWidth += CharWidth(w, fromx + *resWidth, c);
	if (c == _Xaw_atowc(XawLF)) {
	    index++;
	    break;
	}
    }
    *resPos = index;
    *resHeight = ext->max_logical_extent.height;
}


static void
FindPosition(w, fromPos, fromx, width, stopAtWordBreak, resPos, resWidth, resHeight)
    Widget w;
    XawTextPosition fromPos; 	/* Starting position. */
    int fromx;			/* Horizontal location of starting position.*/
    int width;			/* Desired width. */
    int stopAtWordBreak;	/* Whether the resulting position should be at
				   a word break. */
    XawTextPosition *resPos;	/* Resulting position. */
    int* resWidth;		/* Actual width used. */
    int* resHeight;		/* Height required. */
{
    MultiSinkObject sink = (MultiSinkObject) w;
    Widget source = XawTextGetSource(XtParent(w));

    XawTextPosition lastPos, index, whiteSpacePosition = 0;
    int     lastWidth = 0, whiteSpaceWidth = 0;
    Boolean whiteSpaceSeen;
    wchar_t c;
    XFontSetExtents *ext = XExtentsOfFontSet(sink->multi_sink.fontset);
    XawTextBlock blk;

    lastPos = GETLASTPOS;

    XawTextSourceRead(source, fromPos, &blk, BUFSIZ);
    *resWidth = 0;
    whiteSpaceSeen = FALSE;
    c = 0;
    for (index = fromPos; *resWidth <= width && index < lastPos; index++) {
	lastWidth = *resWidth;
	if (index - blk.firstPos >= blk.length)
	    XawTextSourceRead(source, index, &blk, BUFSIZ);
        c = ((wchar_t *)blk.ptr)[index - blk.firstPos];
        *resWidth += CharWidth(w, fromx + *resWidth, c);

        if ((c == _Xaw_atowc(XawSP) || c == _Xaw_atowc(XawTAB)) && 
	    *resWidth <= width) {
	    whiteSpaceSeen = TRUE;
	    whiteSpacePosition = index;
	    whiteSpaceWidth = *resWidth;
	}
	if (c == _Xaw_atowc(XawLF)) {
	    index++;
	    break;
	}
    }
    if (*resWidth > width && index > fromPos) {
	*resWidth = lastWidth;
	index--;
	if (stopAtWordBreak && whiteSpaceSeen) {
	    index = whiteSpacePosition + 1;
	    *resWidth = whiteSpaceWidth;
	}
    }
    if (index == lastPos && c != _Xaw_atowc(XawLF)) index = lastPos + 1;
    *resPos = index;
    *resHeight = ext->max_logical_extent.height;
}

static void
Resolve (w, pos, fromx, width, leftPos, rightPos)
    Widget w;
    XawTextPosition pos;
    int fromx, width;
    XawTextPosition *leftPos, *rightPos;
{
    int resWidth, resHeight;
    Widget source = XawTextGetSource(XtParent(w));

    FindPosition(w, pos, fromx, width, FALSE, leftPos, &resWidth, &resHeight);
    if (*leftPos > GETLASTPOS)
      *leftPos = GETLASTPOS;
    *rightPos = *leftPos;
}

static void
GetGC(sink)
    MultiSinkObject sink;
{
    XtGCMask valuemask = (GCGraphicsExposures | GCForeground | GCBackground );
    XGCValues values;

    values.graphics_exposures = (Bool) FALSE;
    
    values.foreground = sink->text_sink.foreground;
    values.background = sink->text_sink.background;

    sink->multi_sink.normgc = XtAllocateGC( (Widget)sink, 0, valuemask, &values, GCFont, 0 );
    
    values.foreground = sink->text_sink.background;
    values.background = sink->text_sink.foreground;
    sink->multi_sink.invgc = XtAllocateGC( (Widget)sink, 0, valuemask, &values, GCFont, 0 );
    
    values.function = GXxor;
    values.background = (unsigned long) 0L;	/* (pix ^ 0) = pix */
    values.foreground = (sink->text_sink.background ^ 
			 sink->text_sink.foreground);
    valuemask = GCGraphicsExposures | GCFunction | GCForeground | GCBackground;
    
    /* if this GC is not used for fontset rendering then AllocateGC aint needed. Dont hurt tho.*/
    sink->multi_sink.xorgc = XtAllocateGC( (Widget)sink, 0, valuemask, &values, GCFont, 0 );
}


/***** Public routines *****/

/*	Function Name: Initialize
 *	Description: Initializes the TextSink Object.
 *	Arguments: request, new - the requested and new values for the object
 *                                instance.
 *	Returns: none.
 *
 */

/* ARGSUSED */
static void
Initialize(request, new, args, num_args)
    Widget request, new;
    ArgList args;
    Cardinal* num_args;
{
    MultiSinkObject sink = (MultiSinkObject) new;

    GetGC(sink);
    
    sink->multi_sink.insertCursorOn= CreateInsertCursor(XtScreenOfObject(new));
    sink->multi_sink.laststate = XawisOff;
    sink->multi_sink.cursor_x = sink->multi_sink.cursor_y = 0;
}

/*	Function Name: Destroy
 *	Description: This function cleans up when the object is 
 *                   destroyed.
 *	Arguments: w - the MultiSink Object.
 *	Returns: none.
 */

static void
Destroy(w)
    Widget w;
{
   MultiSinkObject sink = (MultiSinkObject) w;

   XtReleaseGC(w, sink->multi_sink.normgc);
   XtReleaseGC(w, sink->multi_sink.invgc);
   XtReleaseGC(w, sink->multi_sink.xorgc);

   XFreePixmap(XtDisplayOfObject(w), sink->multi_sink.insertCursorOn);
}

/*	Function Name: SetValues
 *	Description: Sets the values for the MultiSink
 *	Arguments: current - current state of the object.
 *                 request - what was requested.
 *                 new - what the object will become.
 *	Returns: True if redisplay is needed.
 */

/* ARGSUSED */
static Boolean
SetValues(current, request, new, args, num_args)
    Widget current, request, new;
    ArgList args;
    Cardinal* num_args;
{
    MultiSinkObject w = (MultiSinkObject) new;
    MultiSinkObject old_w = (MultiSinkObject) current;

    /* Font set is not in the GC! Do not make a new GC when font set changes! */

    if ( w->multi_sink.fontset != old_w->multi_sink.fontset ) {
	((TextWidget)XtParent(new))->text.redisplay_needed = True;
#ifndef NO_TAB_FIX
	SetTabs( w, w->text_sink.tab_count, w->text_sink.char_tabs );
#endif
    }

    if (   w->text_sink.background != old_w->text_sink.background ||
	   w->text_sink.foreground != old_w->text_sink.foreground     ) {

	XtReleaseGC((Widget)w, w->multi_sink.normgc);
	XtReleaseGC((Widget)w, w->multi_sink.invgc);
	XtReleaseGC((Widget)w, w->multi_sink.xorgc);
	GetGC(w);
	((TextWidget)XtParent(new))->text.redisplay_needed = True;
    } else {
	if ( (w->multi_sink.echo != old_w->multi_sink.echo) ||
	     (w->multi_sink.display_nonprinting != 
                                     old_w->multi_sink.display_nonprinting) )
	    ((TextWidget)XtParent(new))->text.redisplay_needed = True;
    }
    
    return False;
}

/*	Function Name: MaxLines
 *	Description: Finds the Maximum number of lines that will fit in
 *                   a given height.
 *	Arguments: w - the MultiSink Object.
 *                 height - height to fit lines into.
 *	Returns: the number of lines that will fit.
 */

/* ARGSUSED */
static int
MaxLines(w, height)
    Widget w;
    Dimension height;
{
  MultiSinkObject sink = (MultiSinkObject) w;
  int font_height;
  XFontSetExtents *ext = XExtentsOfFontSet(sink->multi_sink.fontset);

  font_height = ext->max_logical_extent.height;
  return( ((int) height) / font_height );
}

/*	Function Name: MaxHeight
 *	Description: Finds the Minium height that will contain a given number 
 *                   lines.
 *	Arguments: w - the MultiSink Object.
 *                 lines - the number of lines.
 *	Returns: the height.
 */

/* ARGSUSED */
static int
#if NeedFunctionPrototypes
MaxHeight( 
    Widget w, 
    int lines )
#else
MaxHeight( w, lines )
    Widget w;
    int lines;
#endif
{
  MultiSinkObject sink = (MultiSinkObject) w;
  XFontSetExtents *ext = XExtentsOfFontSet(sink->multi_sink.fontset);

  return(lines * ext->max_logical_extent.height); 
}

/*	Function Name: SetTabs
 *	Description: Sets the Tab stops.
 *	Arguments: w - the MultiSink Object.
 *                 tab_count - the number of tabs in the list.
 *                 tabs - the text positions of the tabs.
 *	Returns: none
 */

static void 
#if NeedFunctionPrototypes
SetTabs( 
    Widget w, 
    int tab_count, 
    short* tabs )
#else
SetTabs( w, tab_count, tabs )
    Widget w;
    int tab_count;
    short* tabs;
#endif
{
  MultiSinkObject sink = (MultiSinkObject) w;
  int i;
  Atom XA_FIGURE_WIDTH;
  unsigned long figure_width = 0;
  XFontStruct *font;

  /*
   * Bug:
   *   Suppose the first font of fontset stores the unit of column.
   *
   * By Li Yuhong, Mar. 14, 1991
   */
  { XFontStruct **f_list;
    char	**f_name;

    (void) XFontsOfFontSet(sink->multi_sink.fontset, &f_list, &f_name);
    font = f_list[0];
  }

/*
 * Find the figure width of the current font.
 */

  XA_FIGURE_WIDTH = XInternAtom(XtDisplayOfObject(w), "FIGURE_WIDTH", FALSE);
  if ( (XA_FIGURE_WIDTH != None) && 
       ( (!XGetFontProperty(font, XA_FIGURE_WIDTH, &figure_width)) ||
	 (figure_width == 0)) ) 
    if (font->per_char && font->min_char_or_byte2 <= '$' &&
	font->max_char_or_byte2 >= '$')
      figure_width = font->per_char['$' - font->min_char_or_byte2].width;
    else
      figure_width = font->max_bounds.width;

  if (tab_count > sink->text_sink.tab_count) {
    sink->text_sink.tabs = (Position *)
	XtRealloc((char *) sink->text_sink.tabs,
		  (Cardinal) (tab_count * sizeof(Position)));
    sink->text_sink.char_tabs = (short *)
	XtRealloc((char *) sink->text_sink.char_tabs,
		  (Cardinal) (tab_count * sizeof(short)));
  }

  for ( i = 0 ; i < tab_count ; i++ ) {
    sink->text_sink.tabs[i] = tabs[i] * figure_width;
    sink->text_sink.char_tabs[i] = tabs[i];
  }
    
  sink->text_sink.tab_count = tab_count;

#ifndef NO_TAB_FIX
  ((TextWidget)XtParent(w))->text.redisplay_needed = True;
#endif
}

void
#if NeedFunctionPrototypes
_XawMultiSinkPosToXY(
    Widget w, 
    XawTextPosition pos, 
    Position *x,
    Position *y )
#else
_XawMultiSinkPosToXY( w, pos, x, y )
    Widget w;
    XawTextPosition pos;
    Position *x, *y;
#endif
{
    MultiSinkObject sink = (MultiSinkObject) ((TextWidget)w)->text.sink;
    XFontSetExtents *ext = XExtentsOfFontSet( sink->multi_sink.fontset );

    _XawTextPosToXY( w, pos, x, y );
    *y += abs( ext->max_logical_extent.y );
}
