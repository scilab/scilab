/* $XConsortium: Text.c,v 1.197 95/06/14 15:07:27 kaleb Exp $ */

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
#include <X11/Shell.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "XawI18n.h"
#include <stdio.h>

#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/CharSet.h>
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/StdSel.h>
#include <X11/Xmu/Misc.h>

#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/Cardinals.h>
#include <X11/Xaw3d/Scrollbar.h>
#include <X11/Xaw3d/TextP.h>
#include <X11/Xaw3d/MultiSinkP.h>
#include <X11/Xaw3d/XawImP.h>

#include <X11/Xfuncs.h>
#include <ctype.h>		/* for isprint() */

#ifndef MAX_LEN_CT
#define MAX_LEN_CT 6		/* for sequence: ESC $ ( A \xx \xx */
#endif

unsigned long FMT8BIT = 0L;
unsigned long XawFmt8Bit = 0L;
unsigned long XawFmtWide = 0L;

#define SinkClearToBG          XawTextSinkClearToBackground

#define SrcScan                XawTextSourceScan
#define SrcRead                XawTextSourceRead
#define SrcReplace             XawTextSourceReplace
#define SrcSearch              XawTextSourceSearch
#define SrcCvtSel              XawTextSourceConvertSelection
#define SrcSetSelection        XawTextSourceSetSelection

#define BIGNUM ((Dimension)32023)
#define MULTI_CLICK_TIME 500L

/*
 * Compute a the maximum length of a cut buffer that we can pass at any
 * time.  The 64 allows for the overhead of the Change Property request.
 */

#define MAX_CUT_LEN(dpy)  (XMaxRequestSize(dpy) - 64)

#define IsValidLine(ctx, num) ( ((num) == 0) || \
			        ((ctx)->text.lt.info[(num)].position != 0) )

/*
 * Defined in Text.c
 */
static void UnrealizeScrollbars();
static void VScroll(), VJump(), HScroll(), HJump(), ClearWindow(); 
static void DisplayTextWindow(), ModifySelection(), PushCopyQueue();
static void UpdateTextInLine(), UpdateTextInRectangle(), PopCopyQueue();
static void FlushUpdate();
static Boolean LineAndXYForPosition(), TranslateExposeRegion();
static XawTextPosition FindGoodPosition(), _BuildLineTable();

void _XawTextAlterSelection(), _XawTextExecuteUpdate();
void _XawTextSetScrollBars(), _XawTextPrepareToUpdate();

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

static XawTextSelectType defaultSelectTypes[] = {
  XawselectPosition, XawselectWord, XawselectLine, XawselectParagraph,
  XawselectAll,      XawselectNull,
};

static XPointer defaultSelectTypesPtr = (XPointer)defaultSelectTypes;
extern char *_XawDefaultTextTranslations1, *_XawDefaultTextTranslations2,
  *_XawDefaultTextTranslations3, *_XawDefaultTextTranslations4;
static Dimension defWidth = 100;
static Dimension defHeight = DEFAULT_TEXT_HEIGHT;

#define offset(field) XtOffsetOf(TextRec, field)
static XtResource resources[] = {
  {XtNwidth, XtCWidth, XtRDimension, sizeof(Dimension),
     offset(core.width), XtRDimension, (XtPointer)&defWidth},
  {XtNcursor, XtCCursor, XtRCursor, sizeof(Cursor),
     offset(simple.cursor), XtRString, "xterm"},
  {XtNheight, XtCHeight, XtRDimension, sizeof(Dimension),
     offset(core.height), XtRDimension, (XtPointer)&defHeight},
  {XtNdisplayPosition, XtCTextPosition, XtRInt, sizeof(XawTextPosition), 
     offset(text.lt.top), XtRImmediate, (XtPointer)0},
  {XtNinsertPosition, XtCTextPosition, XtRInt, sizeof(XawTextPosition),
     offset(text.insertPos), XtRImmediate,(XtPointer)0},
  {XtNleftMargin, XtCMargin, XtRPosition, sizeof (Position),
     offset(text.r_margin.left), XtRImmediate, (XtPointer)2},
  {XtNrightMargin, XtCMargin, XtRPosition, sizeof (Position),
     offset(text.r_margin.right), XtRImmediate, (XtPointer)4},
  {XtNtopMargin, XtCMargin, XtRPosition, sizeof (Position),
     offset(text.r_margin.top), XtRImmediate, (XtPointer)2},
  {XtNbottomMargin, XtCMargin, XtRPosition, sizeof (Position),
     offset(text.r_margin.bottom), XtRImmediate, (XtPointer)2},
  {XtNselectTypes, XtCSelectTypes, XtRPointer,
     sizeof(XawTextSelectType*), offset(text.sarray),
     XtRPointer, (XtPointer)&defaultSelectTypesPtr},
  {XtNtextSource, XtCTextSource, XtRWidget, sizeof (Widget),
     offset(text.source), XtRImmediate, NULL},
  {XtNtextSink, XtCTextSink, XtRWidget, sizeof (Widget),
     offset(text.sink), XtRImmediate, NULL},
  {XtNdisplayCaret, XtCOutput, XtRBoolean, sizeof(Boolean),
     offset(text.display_caret), XtRImmediate, (XtPointer)True},
  {XtNscrollVertical, XtCScroll, XtRScrollMode, sizeof(XawTextScrollMode),
     offset(text.scroll_vert), XtRImmediate, (XtPointer) XawtextScrollNever},
  {XtNscrollHorizontal, XtCScroll, XtRScrollMode, sizeof(XawTextScrollMode),
     offset(text.scroll_horiz), XtRImmediate, (XtPointer) XawtextScrollNever},
  {XtNwrap, XtCWrap, XtRWrapMode, sizeof(XawTextWrapMode),
     offset(text.wrap), XtRImmediate, (XtPointer) XawtextWrapNever},
  {XtNresize, XtCResize, XtRResizeMode, sizeof(XawTextResizeMode),
     offset(text.resize), XtRImmediate, (XtPointer) XawtextResizeNever},
  {XtNautoFill, XtCAutoFill, XtRBoolean, sizeof(Boolean),
     offset(text.auto_fill), XtRImmediate, (XtPointer) FALSE},
  {XtNunrealizeCallback, XtCCallback, XtRCallback, sizeof(XtPointer),
     offset(text.unrealize_callbacks), XtRCallback, (XtPointer) NULL}
};
#undef offset

/* ARGSUSED */
static void 
CvtStringToScrollMode(args, num_args, fromVal, toVal)
XrmValuePtr args;		/* unused */
Cardinal	*num_args;	/* unused */
XrmValuePtr	fromVal;
XrmValuePtr	toVal;
{
  static XawTextScrollMode scrollMode;
  static  XrmQuark  QScrollNever, QScrollAlways, QScrollWhenNeeded;
  XrmQuark    q;
  char        lowerName[40];
  static Boolean inited = FALSE;
    
  if ( !inited ) {
    QScrollNever      = XrmPermStringToQuark(XtEtextScrollNever);
    QScrollWhenNeeded = XrmPermStringToQuark(XtEtextScrollWhenNeeded);
    QScrollAlways     = XrmPermStringToQuark(XtEtextScrollAlways);
    inited = TRUE;
  }

  if (strlen ((char*) fromVal->addr) < sizeof lowerName) {
    XmuCopyISOLatin1Lowered (lowerName, (char *)fromVal->addr);
    q = XrmStringToQuark(lowerName);

    if      (q == QScrollNever)          scrollMode = XawtextScrollNever;
    else if (q == QScrollWhenNeeded)     scrollMode = XawtextScrollWhenNeeded;
    else if (q == QScrollAlways)         scrollMode = XawtextScrollAlways;
    else {
      toVal->size = 0;
      toVal->addr = NULL;
      return;
    }
    toVal->size = sizeof scrollMode;
    toVal->addr = (XPointer) &scrollMode;
    return;
  }
  toVal->size = 0;
  toVal->addr = NULL;
}

/* ARGSUSED */
static void 
CvtStringToWrapMode(args, num_args, fromVal, toVal)
XrmValuePtr args;		/* unused */
Cardinal	*num_args;	/* unused */
XrmValuePtr	fromVal;
XrmValuePtr	toVal;
{
  static XawTextWrapMode wrapMode;
  static  XrmQuark QWrapNever, QWrapLine, QWrapWord;
  XrmQuark    q;
  char        lowerName[BUFSIZ];
  static Boolean inited = FALSE;
    
  if ( !inited ) {
    QWrapNever = XrmPermStringToQuark(XtEtextWrapNever);
    QWrapLine  = XrmPermStringToQuark(XtEtextWrapLine);
    QWrapWord  = XrmPermStringToQuark(XtEtextWrapWord);
    inited = TRUE;
  }

  if (strlen ((char*) fromVal->addr) < sizeof lowerName) {
    XmuCopyISOLatin1Lowered (lowerName, (char *)fromVal->addr);
    q = XrmStringToQuark(lowerName);

    if      (q == QWrapNever)     wrapMode = XawtextWrapNever;
    else if (q == QWrapLine)      wrapMode = XawtextWrapLine;
    else if (q == QWrapWord)      wrapMode = XawtextWrapWord;
    else {
      toVal->size = 0;
      toVal->addr = NULL;
      return;
    }
    toVal->size = sizeof wrapMode;
    toVal->addr = (XPointer) &wrapMode;
    return;
  }
  toVal->size = 0;
  toVal->addr = NULL;
}

/* ARGSUSED */
static void 
CvtStringToResizeMode(args, num_args, fromVal, toVal)
XrmValuePtr args;		/* unused */
Cardinal	*num_args;	/* unused */
XrmValuePtr	fromVal;
XrmValuePtr	toVal;
{
  static XawTextResizeMode resizeMode;
  static  XrmQuark  QResizeNever, QResizeWidth, QResizeHeight, QResizeBoth;
  XrmQuark    q;
  char        lowerName[40];
  static Boolean inited = FALSE;
    
  if ( !inited ) {
    QResizeNever      = XrmPermStringToQuark(XtEtextResizeNever);
    QResizeWidth      = XrmPermStringToQuark(XtEtextResizeWidth);
    QResizeHeight     = XrmPermStringToQuark(XtEtextResizeHeight);
    QResizeBoth       = XrmPermStringToQuark(XtEtextResizeBoth);
    inited = TRUE;
  }

  if (strlen ((char*) fromVal->addr) < sizeof lowerName) {
    XmuCopyISOLatin1Lowered (lowerName, (char *)fromVal->addr);
    q = XrmStringToQuark(lowerName);

    if      (q == QResizeNever)          resizeMode = XawtextResizeNever;
    else if (q == QResizeWidth)          resizeMode = XawtextResizeWidth;
    else if (q == QResizeHeight)         resizeMode = XawtextResizeHeight;
    else if (q == QResizeBoth)           resizeMode = XawtextResizeBoth;
    else {
      toVal->size = 0;
      toVal->addr = NULL;
      return;
    }
    toVal->size = sizeof resizeMode;
    toVal->addr = (XPointer) &resizeMode;
    return;
  }
  toVal->size = 0;
  toVal->addr = NULL;
}

static void 
ClassInitialize()
{
  int len1 = strlen (_XawDefaultTextTranslations1);
  int len2 = strlen (_XawDefaultTextTranslations2);
  int len3 = strlen (_XawDefaultTextTranslations3);
  int len4 = strlen (_XawDefaultTextTranslations4);
  char *buf = XtMalloc ((unsigned)(len1 + len2 + len3 + len4 + 1));
  char *cp = buf;

  if (!XawFmt8Bit)
    FMT8BIT = XawFmt8Bit = XrmPermStringToQuark("FMT8BIT");
  if (!XawFmtWide)
    XawFmtWide = XrmPermStringToQuark("FMTWIDE");

  XawInitializeWidgetSet();

/* 
 * Set the number of actions.
 */

  textClassRec.core_class.num_actions = _XawTextActionsTableCount;
  
  (void) strcpy( cp, _XawDefaultTextTranslations1); cp += len1;
  (void) strcpy( cp, _XawDefaultTextTranslations2); cp += len2;
  (void) strcpy( cp, _XawDefaultTextTranslations3); cp += len3;
  (void) strcpy( cp, _XawDefaultTextTranslations4);
  textWidgetClass->core_class.tm_table = buf;

  XtAddConverter(XtRString, XtRScrollMode, CvtStringToScrollMode,
			(XtConvertArgList)NULL, (Cardinal)0 );
  XtAddConverter(XtRString, XtRWrapMode,   CvtStringToWrapMode,
			(XtConvertArgList)NULL, (Cardinal)0 );
  XtAddConverter(XtRString, XtRResizeMode, CvtStringToResizeMode,
			(XtConvertArgList)NULL, (Cardinal)0 );
}

/*	Function Name: PositionHScrollBar.
 *	Description: Positions the Horizontal scrollbar.
 *	Arguments: ctx - the text widget.
 *	Returns: none
 */

static void
PositionHScrollBar(ctx)
TextWidget ctx;
{
  Widget vbar = ctx->text.vbar, hbar = ctx->text.hbar;
  Position top, left = 0;

  if (ctx->text.hbar == NULL) return;

  if (vbar != NULL)
    left += (Position) (vbar->core.width + vbar->core.border_width);

  XtResizeWidget( hbar, ctx->core.width - left, hbar->core.height,
		 hbar->core.border_width );

  left -= (Position) hbar->core.border_width;
  
  top = ctx->core.height - ( hbar->core.height + hbar->core.border_width);
  XtMoveWidget( hbar, left, top); 
}

/*	Function Name: PositionVScrollBar.
 *	Description: Positions the Vertical scrollbar.
 *	Arguments: ctx - the text widget.
 *	Returns: none.
 */

static void
PositionVScrollBar(ctx)
TextWidget ctx;
{
  Widget vbar = ctx->text.vbar;
  Dimension bw;

  if (vbar == NULL) return;
  bw = vbar->core.border_width;

  XtResizeWidget( vbar, vbar->core.width, ctx->core.height, bw);
  XtMoveWidget( vbar, -(Position)bw, -(Position)bw );
}

static void 
CreateVScrollBar(ctx)
TextWidget ctx;
{
  Widget vbar;

  if (ctx->text.vbar != NULL) return;

  ctx->text.vbar = vbar =
    XtCreateWidget("vScrollbar", scrollbarWidgetClass, (Widget)ctx,
		(ArgList) NULL, ZERO);
  XtAddCallback( vbar, XtNscrollProc, VScroll, (XtPointer)ctx );
  XtAddCallback( vbar, XtNjumpProc, VJump, (XtPointer)ctx );
  if (ctx->text.hbar == NULL)
      XtAddCallback((Widget) ctx, XtNunrealizeCallback, UnrealizeScrollbars,
		    (XtPointer) NULL);

  ctx->text.r_margin.left += vbar->core.width + vbar->core.border_width;
  ctx->text.margin.left = ctx->text.r_margin.left;

  PositionVScrollBar(ctx);
  PositionHScrollBar(ctx);	/* May modify location of Horiz. Bar. */

  if (XtIsRealized((Widget)ctx)) {
    XtRealizeWidget(vbar);
    XtMapWidget(vbar);
  }
}

/*	Function Name: DestroyVScrollBar
 *	Description: Removes a vertical ScrollBar.
 *	Arguments: ctx - the parent text widget.
 *	Returns: none.
 */

static void
DestroyVScrollBar(ctx)
TextWidget ctx;
{
  Widget vbar = ctx->text.vbar;

  if (vbar == NULL) return;

  ctx->text.r_margin.left -= vbar->core.width + vbar->core.border_width;
  ctx->text.margin.left = ctx->text.r_margin.left;
  if (ctx->text.hbar == NULL)
      XtRemoveCallback((Widget) ctx, XtNunrealizeCallback, UnrealizeScrollbars,
		       (XtPointer) NULL);
  XtDestroyWidget(vbar);
  ctx->text.vbar = NULL;
  PositionHScrollBar(ctx);
}
  
static void 
CreateHScrollBar(ctx)
TextWidget ctx;
{
  Arg args[1];
  Widget hbar;

  if (ctx->text.hbar != NULL) return;

  XtSetArg(args[0], XtNorientation, XtorientHorizontal);
  ctx->text.hbar = hbar =
    XtCreateWidget("hScrollbar", scrollbarWidgetClass, (Widget)ctx, args, ONE);
  XtAddCallback( hbar, XtNscrollProc, HScroll, (XtPointer)ctx );
  XtAddCallback( hbar, XtNjumpProc, HJump, (XtPointer)ctx );
  if (ctx->text.vbar == NULL)
      XtAddCallback((Widget) ctx, XtNunrealizeCallback, UnrealizeScrollbars,
		    (XtPointer) NULL);

  PositionHScrollBar(ctx);
  if (XtIsRealized((Widget)ctx)) {
    XtRealizeWidget(hbar);
    XtMapWidget(hbar);
  }
}

/*	Function Name: DestroyHScrollBar
 *	Description: Removes a horizontal ScrollBar.
 *	Arguments: ctx - the parent text widget.
 *	Returns: none.
 */

static void
DestroyHScrollBar(ctx)
TextWidget ctx;
{
  Widget hbar = ctx->text.hbar;

  if (hbar == NULL) return;

/*
  ctx->text.r_margin.bottom -= hbar->core.height + hbar->core.border_width;
  ctx->text.margin.bottom = ctx->text.r_margin.bottom;
*/
  if (ctx->text.vbar == NULL)
      XtRemoveCallback((Widget) ctx, XtNunrealizeCallback, UnrealizeScrollbars,
		       (XtPointer) NULL);
  XtDestroyWidget(hbar);
  ctx->text.hbar = NULL;
}

/* ARGSUSED */
static void 
Initialize(request, new, args, num_args)
Widget request, new;
ArgList args;			/* unused */
Cardinal *num_args;		/* unused */
{
  TextWidget ctx = (TextWidget) new;
  char error_buf[BUFSIZ];

  ctx->text.lt.lines = 0;
  ctx->text.lt.info = NULL;
  (void) bzero((char *) &(ctx->text.origSel), sizeof(XawTextSelection));
  (void) bzero((char *) &(ctx->text.s), sizeof(XawTextSelection)); 
  ctx->text.s.type = XawselectPosition;
  ctx->text.salt = NULL;
  ctx->text.hbar = ctx->text.vbar = (Widget) NULL;
  ctx->text.lasttime = 0; /* ||| correct? */
  ctx->text.time = 0; /* ||| correct? */
  ctx->text.showposition = TRUE;
  ctx->text.lastPos = (ctx->text.source != NULL) ? GETLASTPOS : 0;
  ctx->text.file_insert = NULL;
  ctx->text.search = NULL;
  ctx->text.updateFrom = (XawTextPosition *) XtMalloc((unsigned) ONE);
  ctx->text.updateTo = (XawTextPosition *) XtMalloc((unsigned) ONE);
  ctx->text.numranges = ctx->text.maxranges = 0;
  ctx->text.gc = DefaultGCOfScreen(XtScreen(ctx)); 
  ctx->text.hasfocus = FALSE;
  ctx->text.margin = ctx->text.r_margin; /* Strucure copy. */
  ctx->text.update_disabled = FALSE;
  ctx->text.old_insert = -1;
  ctx->text.mult = 1;
  ctx->text.single_char = FALSE;
  ctx->text.copy_area_offsets = NULL;
  ctx->text.salt2 = NULL;

  if (ctx->core.height == DEFAULT_TEXT_HEIGHT) {
    ctx->core.height = VMargins(ctx);
    if (ctx->text.sink != NULL)
      ctx->core.height += XawTextSinkMaxHeight(ctx->text.sink, 1);
  }

  if (ctx->text.scroll_vert != XawtextScrollNever) 
    if ( (ctx->text.resize == XawtextResizeHeight) ||
     	 (ctx->text.resize == XawtextResizeBoth) ) {
      (void) sprintf(error_buf, "Xaw Text Widget %s:\n %s %s.", ctx->core.name,
	      "Vertical scrolling not allowed with height resize.\n",
	      "Vertical scrolling has been DEACTIVATED.");
      XtAppWarning(XtWidgetToApplicationContext(new), error_buf);
      ctx->text.scroll_vert = XawtextScrollNever;
    }
    else if (ctx->text.scroll_vert == XawtextScrollAlways)
      CreateVScrollBar(ctx);

  if (ctx->text.scroll_horiz != XawtextScrollNever) 
    if (ctx->text.wrap != XawtextWrapNever) {
      (void) sprintf(error_buf, "Xaw Text Widget %s:\n %s %s.", ctx->core.name,
	      "Horizontal scrolling not allowed with wrapping active.\n",
	      "Horizontal scrolling has been DEACTIVATED.");
      XtAppWarning(XtWidgetToApplicationContext(new), error_buf);
      ctx->text.scroll_horiz = XawtextScrollNever;
    }
    else if ( (ctx->text.resize == XawtextResizeWidth) ||
	      (ctx->text.resize == XawtextResizeBoth) ) {
      (void) sprintf(error_buf, "Xaw Text Widget %s:\n %s %s.", ctx->core.name,
	      "Horizontal scrolling not allowed with width resize.\n",
	      "Horizontal scrolling has been DEACTIVATED.");
      XtAppWarning(XtWidgetToApplicationContext(new), error_buf);
      ctx->text.scroll_horiz = XawtextScrollNever;
    }
    else if (ctx->text.scroll_horiz == XawtextScrollAlways)
      CreateHScrollBar(ctx);
}

static void 
Realize( w, valueMask, attributes )
Widget w;
Mask *valueMask;
XSetWindowAttributes *attributes;
{
  TextWidget ctx = (TextWidget)w;
  void _XawTextCheckResize();

  (*textClassRec.core_class.superclass->core_class.realize)
    (w, valueMask, attributes);
  
  if (ctx->text.hbar != NULL) {	        /* Put up Hbar -- Must be first. */
    XtRealizeWidget(ctx->text.hbar);
    XtMapWidget(ctx->text.hbar);
  }

  if (ctx->text.vbar != NULL) {	        /* Put up Vbar. */
    XtRealizeWidget(ctx->text.vbar);
    XtMapWidget(ctx->text.vbar);
  }

  _XawTextBuildLineTable(ctx, ctx->text.lt.top, TRUE);
  _XawTextSetScrollBars(ctx);
  _XawTextCheckResize(ctx);
}

/*ARGSUSED*/
static void UnrealizeScrollbars(widget, client, call)
Widget		widget;		/* Text widget */
XtPointer	client;		/* unused */
XtPointer	call; 		/* unused */
{
    TextWidget ctx = (TextWidget) widget;
    
    if (ctx->text.hbar)
	XtUnrealizeWidget(ctx->text.hbar);
    if (ctx->text.vbar)
	XtUnrealizeWidget(ctx->text.vbar);
}

/* Utility routines for support of Text */

static void
_CreateCutBuffers(d)
Display *d;
{
  static struct _DisplayRec {
    struct _DisplayRec *next;
    Display *dpy;
  } *dpy_list = NULL;
  struct _DisplayRec *dpy_ptr;

  for (dpy_ptr = dpy_list; dpy_ptr != NULL; dpy_ptr = dpy_ptr->next)
    if (dpy_ptr->dpy == d) return;

  dpy_ptr = XtNew(struct _DisplayRec);
  dpy_ptr->next = dpy_list;
  dpy_ptr->dpy = d;
  dpy_list = dpy_ptr;

#define Create(buffer) \
    XChangeProperty(d, RootWindow(d, 0), buffer, XA_STRING, 8, \
		    PropModeAppend, NULL, 0 );

    Create( XA_CUT_BUFFER0 );
    Create( XA_CUT_BUFFER1 );
    Create( XA_CUT_BUFFER2 );
    Create( XA_CUT_BUFFER3 );
    Create( XA_CUT_BUFFER4 );
    Create( XA_CUT_BUFFER5 );
    Create( XA_CUT_BUFFER6 );
    Create( XA_CUT_BUFFER7 );

#undef Create
}

/*
 * Procedure to manage insert cursor visibility for editable text.  It uses
 * the value of ctx->insertPos and an implicit argument. In the event that
 * position is immediately preceded by an eol graphic, then the insert cursor
 * is displayed at the beginning of the next line.
*/
static void 
InsertCursor (w, state)
Widget w;
XawTextInsertState state;
{
  TextWidget ctx = (TextWidget)w;
  Position x, y;
  int line;
  
  if (ctx->text.lt.lines < 1) return;

  if ( LineAndXYForPosition(ctx, ctx->text.insertPos, &line, &x, &y) ) {
    if (line < ctx->text.lt.lines)
      y += (ctx->text.lt.info[line + 1].y - ctx->text.lt.info[line].y) + 1;
    else
      y += (ctx->text.lt.info[line].y - ctx->text.lt.info[line - 1].y) + 1;

    if (ctx->text.display_caret)
      XawTextSinkInsertCursor(ctx->text.sink, x, y, state);
  }
  ctx->text.ev_x = x;
  ctx->text.ev_y = y;

  /* Keep Input Method up to speed  */

  if ( ctx->simple.international ) {
    Arg list[1];

    XtSetArg (list[0], XtNinsertPosition, ctx->text.insertPos);
    _XawImSetValues (w, list, 1);
  }
}

/*
 * Procedure to register a span of text that is no longer valid on the display
 * It is used to avoid a number of small, and potentially overlapping, screen
 * updates. 
*/

void
_XawTextNeedsUpdating(ctx, left, right)
TextWidget ctx;
XawTextPosition left, right;
{
  int i;
  if (left < right) {
    for (i = 0; i < ctx->text.numranges; i++) {
      if (left <= ctx->text.updateTo[i] && right >= ctx->text.updateFrom[i]) {
	ctx->text.updateFrom[i] = Min(left, ctx->text.updateFrom[i]);
	ctx->text.updateTo[i] = Max(right, ctx->text.updateTo[i]);
	return;
      }
    }
    ctx->text.numranges++;
    if (ctx->text.numranges > ctx->text.maxranges) {
      ctx->text.maxranges = ctx->text.numranges;
      i = ctx->text.maxranges * sizeof(XawTextPosition);
      ctx->text.updateFrom = (XawTextPosition *) 
	XtRealloc((char *)ctx->text.updateFrom, (unsigned) i);
      ctx->text.updateTo = (XawTextPosition *) 
	XtRealloc((char *)ctx->text.updateTo, (unsigned) i);
    }
    ctx->text.updateFrom[ctx->text.numranges - 1] = left;
    ctx->text.updateTo[ctx->text.numranges - 1] = right;
  }
}

/*
 * Procedure to read a span of text in Ascii form. This is purely a hack and
 * we probably need to add a function to sources to provide this functionality.
 * [note: this is really a private procedure but is used in multiple modules].
 */

char *
_XawTextGetText(ctx, left, right)
TextWidget ctx;
XawTextPosition left, right;
{
  char *result, *tempResult;
  XawTextBlock text;
  int bytes;

  if (_XawTextFormat(ctx) == XawFmt8Bit)
      bytes = sizeof(unsigned char);
  else if (_XawTextFormat(ctx) == XawFmtWide) 
      bytes = sizeof(wchar_t);
  else /* if there is another fomat, add here */
      bytes = 1;

  /* leave space for ZERO */
  tempResult=result=XtMalloc( (unsigned)(((Cardinal)(right-left))+ONE )* bytes);
  while (left < right) {
    left = SrcRead(ctx->text.source, left, &text, (int)(right - left));
    if (!text.length)
	break;
    memmove(tempResult, text.ptr, text.length * bytes);
    tempResult += text.length * bytes;
  }

  if (bytes == sizeof(wchar_t)) 
      *((wchar_t*)tempResult) = (wchar_t)0;
  else 
      *tempResult = '\0';
  return(result);
}

/* Like _XawTextGetText, but enforces ICCCM STRING type encoding.  This
routine is currently used to put just the ASCII chars in the selection into a
cut buffer. */

char *
_XawTextGetSTRING(ctx, left, right)
TextWidget ctx;
XawTextPosition left, right;
{
  unsigned char *s;
  unsigned char c;
  long i, j, n;
  wchar_t *ws, wc;

  /* allow ESC in accordance with ICCCM */
  if (_XawTextFormat(ctx) == XawFmtWide) {
     MultiSinkObject sink = (MultiSinkObject) ctx->text.sink;
     ws = (wchar_t *)_XawTextGetText(ctx, left, right);
     n = wcslen(ws);
     for (j = 0, i = 0; j < n; j++) {
         wc = ws[j];
         if (XwcTextEscapement (sink->multi_sink.fontset, &wc, 1) || 
            (wc == _Xaw_atowc(XawTAB)) || (wc == _Xaw_atowc(XawLF)) || (wc == _Xaw_atowc(XawESC)))
            ws[i++] = wc;
     }
     ws[i] = (wchar_t)0;
     return (char *)ws;
  } else {
     s = (unsigned char *)_XawTextGetText(ctx, left, right);
     /* only HT and NL control chars are allowed, strip out others */
     n = strlen((char *)s);
     i = 0;
     for (j = 0; j < n; j++) {
	c = s[j];
	if (((c >= 0x20) && c <= 0x7f) ||
	   (c >= 0xa0) || (c == XawTAB) || (c == XawLF) || (c == XawESC)) {
	   s[i] = c;
	   i++;
	}
     }
     s[i] = 0;
     return (char *)s;
  }
#undef ESC

}

/* 
 * This routine maps an x and y position in a window that is displaying text
 * into the corresponding position in the source.
 *
 * NOTE: it is illegal to call this routine unless there is a valid line table!
 */

/*** figure out what line it is on ***/

static XawTextPosition
PositionForXY (ctx, x, y)
TextWidget ctx;
Position x,y;
{
  int fromx, line, width, height;
  XawTextPosition position;

  if (ctx->text.lt.lines == 0) return 0;
  
  for (line = 0; line < ctx->text.lt.lines - 1; line++) {
    if (y <= ctx->text.lt.info[line + 1].y)
      break;
  }
  position = ctx->text.lt.info[line].position;
  if (position >= ctx->text.lastPos)
    return(ctx->text.lastPos);
  fromx = (int) ctx->text.margin.left; 
  XawTextSinkFindPosition( ctx->text.sink, position, fromx, x - fromx,
			  FALSE, &position, &width, &height);
  if (position > ctx->text.lastPos) return(ctx->text.lastPos);
  if (position >= ctx->text.lt.info[line + 1].position)
    position = SrcScan(ctx->text.source, ctx->text.lt.info[line + 1].position,
		       XawstPositions, XawsdLeft, 1, TRUE);
  return(position);
}

/*
 * This routine maps a source position in to the corresponding line number
 * of the text that is displayed in the window.
 *
 * NOTE: It is illegal to call this routine unless there is a valid line table!
 */

static int 
LineForPosition (ctx, position)
TextWidget ctx;
XawTextPosition position;
{
  int line;
  
  for (line = 0; line < ctx->text.lt.lines; line++)
    if (position < ctx->text.lt.info[line + 1].position)
      break;
  return(line);
}

/*
 * This routine maps a source position into the corresponding line number
 * and the x, y coordinates of the text that is displayed in the window.
 *
 * NOTE: It is illegal to call this routine unless there is a valid line table!
 */

static Boolean
LineAndXYForPosition (ctx, pos, line, x, y)
TextWidget ctx;
XawTextPosition pos;
int *line;
Position *x, *y;
{
  XawTextPosition linePos, endPos;
  Boolean visible;
  int realW, realH;

  *line = 0;
  *x = ctx->text.margin.left;
  *y = ctx->text.margin.top;
  if ((visible = IsPositionVisible(ctx, pos))) {
    *line = LineForPosition(ctx, pos);
    *y = ctx->text.lt.info[*line].y;
    *x = ctx->text.margin.left;
    linePos = ctx->text.lt.info[*line].position;
    XawTextSinkFindDistance( ctx->text.sink, linePos,
			    *x, pos, &realW, &endPos, &realH);
    *x += realW;
  }
  return(visible);
}

/*
 * This routine builds a line table. It does this by starting at the
 * specified position and measuring text to determine the staring position
 * of each line to be displayed. It also determines and saves in the
 * linetable all the required metrics for displaying a given line (e.g.
 * x offset, y offset, line length, etc.).
 */

void 
#if NeedFunctionPrototypes
_XawTextBuildLineTable (
    TextWidget ctx,
    XawTextPosition position,
    _XtBoolean force_rebuild)
#else
_XawTextBuildLineTable (ctx, position, force_rebuild)
    TextWidget ctx;
    XawTextPosition position;
    Boolean force_rebuild;
#endif
{
  Dimension height = 0;
  int lines = 0; 
  Cardinal size;

  if ((int)ctx->core.height > VMargins(ctx)) {
    height = ctx->core.height - VMargins(ctx);
    lines = XawTextSinkMaxLines(ctx->text.sink, height);
  }
  size = sizeof(XawTextLineTableEntry) * (lines + 1);

  if ( (lines != ctx->text.lt.lines) || (ctx->text.lt.info == NULL) ) {
    ctx->text.lt.info = (XawTextLineTableEntry *) XtRealloc((char *) ctx->text.
							    lt.info, size);
    ctx->text.lt.lines = lines;
    force_rebuild = TRUE;
  }

  if ( force_rebuild || (position != ctx->text.lt.top) ) {
    (void) bzero((char *) ctx->text.lt.info, size);
    (void) _BuildLineTable(ctx, ctx->text.lt.top = position, zeroPosition, 0);
  }
}

/*
 * This assumes that the line table does not change size.
 */

static XawTextPosition
_BuildLineTable(ctx, position, min_pos, line)
TextWidget ctx;
XawTextPosition position, min_pos;	
int line;
{
  XawTextLineTableEntry * lt = ctx->text.lt.info + line;
  XawTextPosition endPos;
  Position y;
  int count, width, realW, realH;
  Widget src = ctx->text.source;

  if ( ((ctx->text.resize == XawtextResizeWidth) ||
	(ctx->text.resize == XawtextResizeBoth)    ) ||
       (ctx->text.wrap == XawtextWrapNever) )
    width = BIGNUM;
  else 
    width = Max(0, ((int)ctx->core.width - (int)HMargins(ctx)));

  y = ( (line == 0) ? ctx->text.margin.top : lt->y );

  /* CONSTCOND */
  while ( TRUE ) {
    lt->y = y;
    lt->position = position;
    
    XawTextSinkFindPosition( ctx->text.sink, position, ctx->text.margin.left,
			    width, ctx->text.wrap == XawtextWrapWord,
			    &endPos, &realW, &realH);
    lt->textWidth = realW;
    y += realH;

    if (ctx->text.wrap == XawtextWrapNever) 
      endPos = SrcScan(src, position, XawstEOL, XawsdRight, 1, TRUE);

    if ( endPos == ctx->text.lastPos) { /* We have reached the end. */
      if(SrcScan(src, position, XawstEOL, XawsdRight, 1, FALSE) == endPos)
	break;
    }

    ++lt;
    ++line;
    if ( (line > ctx->text.lt.lines) ||
	 ((lt->position == (position = endPos)) && (position > min_pos)) )
      return(position);
  }

/*
 * If we are at the end of the buffer put two special lines in the table.
 *
 * a) Both have position > text.lastPos and lt->textWidth = 0.
 * b) The first has a real height, and the second has a height that
 *    is the rest of the screen.
 *
 * I could fill in the rest of the table with valid heights and a large
 * lastPos, but this method keeps the number of fill regions down to a 
 * minimum.
 *
 * One valid entry is needed at the end of the table so that the cursor
 * does not jump off the bottom of the window.
 */

  for ( count = 0; count < 2 ; count++) 
    if (line++ < ctx->text.lt.lines) { /* make sure not to run of the end. */
      (++lt)->y = (count == 0) ? y : ctx->core.height;
      lt->textWidth = 0;
      lt->position = ctx->text.lastPos + 100;
    }

  if (line < ctx->text.lt.lines) /* Clear out rest of table. */
    (void) bzero( (char *) (lt + 1), 
	  (ctx->text.lt.lines - line) * sizeof(XawTextLineTableEntry) );

  ctx->text.lt.info[ctx->text.lt.lines].position = lt->position;

  return(endPos);
}

/*	Function Name: GetWidestLine
 *	Description: Returns the width (in pixels) of the widest line that
 *                   is currently visable.
 *	Arguments: ctx - the text widget.
 *	Returns: the width of the widest line.
 *
 * NOTE: This function requires a valid line table.
 */

static Dimension
GetWidestLine(ctx)
TextWidget ctx;
{
  int i;
  Dimension widest;
  XawTextLineTablePtr lt = &(ctx->text.lt);

  for (i = 0, widest = 1 ; i < lt->lines ; i++)
    if (widest < lt->info[i].textWidth)
      widest = lt->info[i].textWidth;
  
  return(widest);
}

static void
CheckVBarScrolling(ctx)
TextWidget ctx;
{
  float first, last;
  Boolean temp = (ctx->text.vbar == NULL);

  if (ctx->text.scroll_vert == XawtextScrollNever) return;

  if ( (ctx->text.lastPos > 0) && (ctx->text.lt.lines > 0)) {
    first = ctx->text.lt.top;
    first /= (float) ctx->text.lastPos;
    last = ctx->text.lt.info[ctx->text.lt.lines].position;
    if ( ctx->text.lt.info[ctx->text.lt.lines].position < ctx->text.lastPos )
      last /= (float) ctx->text.lastPos;
    else 
      last = 1.0;

    if (ctx->text.scroll_vert == XawtextScrollWhenNeeded) {
      int line;
      XawTextPosition last_pos;
      Position y = ctx->core.height - ctx->text.margin.bottom;	

      if (ctx->text.hbar != NULL)
	y -= (ctx->text.hbar->core.height +
	      2 * ctx->text.hbar->core.border_width);

      last_pos = PositionForXY(ctx, (Position) ctx->core.width, y);
      line = LineForPosition(ctx, last_pos);

      if ( (y < ctx->text.lt.info[line + 1].y) || ((last - first) < 1.0) )
	CreateVScrollBar(ctx);
      else 
	DestroyVScrollBar(ctx);
    }
  
    if (ctx->text.vbar != NULL) 
      XawScrollbarSetThumb(ctx->text.vbar, first, last - first);
  
    if ( (ctx->text.vbar == NULL) != temp) {
      _XawTextNeedsUpdating(ctx, zeroPosition, ctx->text.lastPos);
      if (ctx->text.vbar == NULL)
	_XawTextBuildLineTable (ctx, zeroPosition, FALSE);
    }
  }
  else if (ctx->text.vbar != NULL)
    if (ctx->text.scroll_vert == XawtextScrollWhenNeeded)
      DestroyVScrollBar(ctx);
    else if (ctx->text.scroll_vert == XawtextScrollAlways)
      XawScrollbarSetThumb(ctx->text.vbar, 0.0, 1.0);
}

/*
 * This routine is used by Text to notify an associated scrollbar of the
 * correct metrics (position and shown fraction) for the text being currently
 * displayed in the window.
 */

void 
_XawTextSetScrollBars(ctx)
TextWidget ctx;
{
  float first, last, widest;
  Boolean temp = (ctx->text.hbar == NULL);
  Boolean vtemp = (ctx->text.vbar == NULL);
  
  CheckVBarScrolling(ctx);

  if (ctx->text.scroll_horiz == XawtextScrollNever) return;

  if (ctx->text.vbar != NULL) 
    widest = (int)(ctx->core.width - ctx->text.vbar->core.width -
		   ctx->text.vbar->core.border_width);
  else
    widest = ctx->core.width;
  widest /= (last = GetWidestLine(ctx));
  if (ctx->text.scroll_horiz == XawtextScrollWhenNeeded) 
    if (widest < 1.0)
      CreateHScrollBar(ctx);
    else
      DestroyHScrollBar(ctx);

  if ( (ctx->text.hbar == NULL) != temp ) {
    _XawTextBuildLineTable (ctx, ctx->text.lt.top, TRUE);
    CheckVBarScrolling(ctx);	/* Recheck need for vbar, now that we added
				   or removed the hbar.*/
  }
  
  if (ctx->text.hbar != NULL) {
    first = ctx->text.r_margin.left - ctx->text.margin.left;
    first /= last;
    XawScrollbarSetThumb(ctx->text.hbar, first, widest); 
  }

  if (((ctx->text.hbar == NULL) && (ctx->text.margin.left !=
				   ctx->text.r_margin.left)) ||
      (ctx->text.vbar == NULL) != vtemp)
  {
    ctx->text.margin.left = ctx->text.r_margin.left;
    _XawTextNeedsUpdating(ctx, zeroPosition, ctx->text.lastPos);      
    FlushUpdate(ctx);
  }
}

/*
 * The routine will scroll the displayed text by lines.  If the arg  is
 * positive, move up; otherwise, move down. [note: this is really a private
 * procedure but is used in multiple modules].
 */

void
_XawTextVScroll(ctx, n)
TextWidget ctx;
int n;			
{
  XawTextPosition top, target;
  int y;
  Arg list[1];
  XawTextLineTable * lt = &(ctx->text.lt);

  if (abs(n) > ctx->text.lt.lines) 
    n = (n > 0) ? ctx->text.lt.lines : -ctx->text.lt.lines;

  if (n == 0) return;

  if (n > 0) {
    if ( IsValidLine(ctx, n) )
      top = Min(lt->info[n].position, ctx->text.lastPos);
    else
      top = ctx->text.lastPos;

    y = IsValidLine(ctx, n) ? lt->info[n].y : ctx->core.height;
    _XawTextBuildLineTable(ctx, top, FALSE);
    if (top >= ctx->text.lastPos)
      DisplayTextWindow( (Widget) ctx);
    else {
      XCopyArea(XtDisplay(ctx), XtWindow(ctx), XtWindow(ctx), ctx->text.gc,
		0, y, (int)ctx->core.width, (int)ctx->core.height - y,
		0, ctx->text.margin.top);

      PushCopyQueue(ctx, 0, (int) -y);
      SinkClearToBG(ctx->text.sink, 
		    (Position) 0,
		    (Position) (ctx->text.margin.top + ctx->core.height - y),
		   (Dimension) ctx->core.width, (Dimension) ctx->core.height);

      if (n < lt->lines) n++; /* update descenders at bottom */
      _XawTextNeedsUpdating(ctx, lt->info[lt->lines - n].position, 
			    ctx->text.lastPos);
      _XawTextSetScrollBars(ctx);
    }
  } 
  else {
    XawTextPosition updateTo;
    unsigned int height, clear_height;

    n = -n;
    target = lt->top;
    top = SrcScan(ctx->text.source, target, XawstEOL,
		  XawsdLeft, n+1, FALSE);

    _XawTextBuildLineTable(ctx, top, FALSE);
    y = IsValidLine(ctx, n) ? lt->info[n].y : ctx->core.height;
    updateTo = IsValidLine(ctx, n) ? lt->info[n].position : ctx->text.lastPos;
    if (IsValidLine(ctx, lt->lines - n))
      height = lt->info[lt->lines-n].y - ctx->text.margin.top;
    else if (ctx->core.height - HMargins(ctx))
      height = ctx->core.height - HMargins(ctx);
    else
      height = 0;
    if (y > (int) ctx->text.margin.top)
      clear_height = y - ctx->text.margin.top;
    else
      clear_height = 0;

    if ( updateTo == target ) {
      XCopyArea(XtDisplay(ctx), XtWindow(ctx), XtWindow(ctx), ctx->text.gc, 
		0, ctx->text.margin.top, (int) ctx->core.width, height, 0, y);
      PushCopyQueue(ctx, 0, (int) y);
      SinkClearToBG(ctx->text.sink, (Position) 0, ctx->text.margin.top,
		   (Dimension) ctx->core.width, (Dimension) clear_height);
      
      _XawTextNeedsUpdating(ctx, lt->info[0].position, updateTo);
      _XawTextSetScrollBars(ctx);
    } 
    else if (lt->top != target)
      DisplayTextWindow((Widget)ctx);
  }
  XtSetArg (list[0], XtNinsertPosition, ctx->text.lt.top+ctx->text.lt.lines);
  _XawImSetValues ((Widget) ctx, list, 1);
}

/*ARGSUSED*/
static void 
HScroll(w, closure, callData)
Widget w;
XtPointer closure;		/* TextWidget */
XtPointer callData;		/* #pixels */
{
  TextWidget ctx = (TextWidget) closure;
  Widget tw = (Widget) ctx;
  Position old_left, pixels = (Position)(int) callData;
  XRectangle rect, t_rect;
  
  _XawTextPrepareToUpdate(ctx);

  old_left = ctx->text.margin.left;
  ctx->text.margin.left -= pixels;
  if (ctx->text.margin.left > ctx->text.r_margin.left) {
    ctx->text.margin.left = ctx->text.r_margin.left;
    pixels = old_left - ctx->text.margin.left;
  }
  
  if (pixels > 0) {
    rect.width = (unsigned short) pixels + ctx->text.margin.right;
    rect.x = (short) ctx->core.width - (short) rect.width;
    rect.y = (short) ctx->text.margin.top;
    rect.height = (unsigned short) ctx->core.height - rect.y;

    XCopyArea(XtDisplay(tw), XtWindow(tw), XtWindow(tw), ctx->text.gc,
	      pixels, (int) rect.y,
	      (unsigned int) rect.x, (unsigned int) ctx->core.height,
	      0, (int) rect.y);

    PushCopyQueue(ctx, (int) -pixels, 0);
  }
  else if (pixels < 0) {
    rect.x = 0;

    if (ctx->text.vbar != NULL)
      rect.x += (short) (ctx->text.vbar->core.width +
			 ctx->text.vbar->core.border_width);

    rect.width = (Position) - pixels;
    rect.y = ctx->text.margin.top;
    rect.height = ctx->core.height - rect.y;

    XCopyArea(XtDisplay(tw), XtWindow(tw), XtWindow(tw), ctx->text.gc,
	      (int) rect.x, (int) rect.y,
	      (unsigned int) ctx->core.width - rect.width,
	      (unsigned int) rect.height,
	      (int) rect.x + rect.width, (int) rect.y);

    PushCopyQueue(ctx, (int) rect.width, 0);

/*
 * Redraw the line overflow marks.
 */

    t_rect.x = ctx->core.width - ctx->text.margin.right;
    t_rect.width = ctx->text.margin.right;
    t_rect.y = rect.y;
    t_rect.height = rect.height;
      
    SinkClearToBG(ctx->text.sink, (Position) t_rect.x, (Position) t_rect.y,
		  (Dimension) t_rect.width, (Dimension) t_rect.height);
    
    UpdateTextInRectangle(ctx, &t_rect);
  }

/*  
 * Put in the text that just became visible.
 */

  if ( pixels != 0 ) {
    SinkClearToBG(ctx->text.sink, (Position) rect.x, (Position) rect.y,
		  (Dimension) rect.width, (Dimension) rect.height);
    
    UpdateTextInRectangle(ctx, &rect);
  }
  _XawTextExecuteUpdate(ctx);
  _XawTextSetScrollBars(ctx);
}

/*ARGSUSED*/
static void 
HJump(w, closure, callData)
Widget w;
XtPointer closure, callData; /* closure = TextWidget, callData = percent. */
{
  TextWidget ctx = (TextWidget) closure;
  float * percent = (float *) callData;
  Position new_left, old_left = ctx->text.margin.left;

  long move; /*difference of Positions can be bigger than Position; lint err */

  new_left = ctx->text.r_margin.left;
  new_left -= (Position) (*percent * GetWidestLine(ctx));
  move = old_left - new_left;

  if (abs(move) < (int)ctx->core.width) {
    HScroll(w, (XtPointer) ctx, (XtPointer) move);
    return;
  }
  _XawTextPrepareToUpdate(ctx);
  ctx->text.margin.left = new_left;
  if (XtIsRealized((Widget) ctx)) DisplayTextWindow((Widget) ctx); 
  _XawTextExecuteUpdate(ctx);
}

/*	Function Name: UpdateTextInLine
 *	Description: Updates some text in a given line.
 *	Arguments: ctx - the text widget.
 *                 line - the line number (in the line table) of this line.
 *                 left, right - left and right pixel offsets of the
 *                               area to update.
 *	Returns: none.
 */

static void
UpdateTextInLine(ctx, line, left, right)
TextWidget ctx;
int line;
Position left, right;
{
  XawTextPosition pos1, pos2; 
  int width, height, local_left, local_width;
  XawTextLineTableEntry * lt = ctx->text.lt.info + line;

  if ( ((int)(lt->textWidth + ctx->text.margin.left) < left) ||
       ( ctx->text.margin.left > right ) )
    return;			/* no need to update. */

  local_width = left - ctx->text.margin.left;
  XawTextSinkFindPosition(ctx->text.sink, lt->position,
			  (int) ctx->text.margin.left, 
			  local_width, FALSE, &pos1, &width, &height);

  if (right >= (Position) lt->textWidth - ctx->text.margin.left) 
    if ( (IsValidLine(ctx, line + 1)) &&
	 (ctx->text.lt.info[line + 1].position <= ctx->text.lastPos) )
      pos2 = SrcScan( ctx->text.source, (lt + 1)->position, XawstPositions, 
			   XawsdLeft, 1, TRUE);
    else 
      pos2 = GETLASTPOS;
  else {
    XawTextPosition t_pos;

    local_left = ctx->text.margin.left + width;
    local_width = right  - local_left;
    XawTextSinkFindPosition(ctx->text.sink, pos1, local_left,
			    local_width, FALSE, &pos2, &width, &height);
    
    t_pos = SrcScan( ctx->text.source, pos2,
		     XawstPositions, XawsdRight, 1, TRUE);
    if (t_pos < (lt + 1)->position)
      pos2 = t_pos;
  }

  _XawTextNeedsUpdating(ctx, pos1, pos2);
}

/*
 * The routine will scroll the displayed text by pixels.  If the calldata is
 * positive, move up; otherwise, move down.
 */

/*ARGSUSED*/
static void 
VScroll(w, closure, callData)
Widget w;
XtPointer closure;		/* TextWidget */
XtPointer callData;		/* #pixels */
{
  TextWidget ctx = (TextWidget)closure;
  int height, nlines, lines = (int) callData;

  height = ctx->core.height - VMargins(ctx);
  if (height < 1)
    height = 1;
  nlines = (int) (lines * (int) ctx->text.lt.lines) / height;
#ifdef ARROW_SCROLLBAR
  if (nlines == 0 && lines != 0) 
    nlines = lines > 0 ? 1 : -1;
#endif
  _XawTextPrepareToUpdate(ctx);
  _XawTextVScroll(ctx, nlines);
  _XawTextExecuteUpdate(ctx);
}

/*
 * The routine "thumbs" the displayed text. Thumbing means reposition the
 * displayed view of the source to a new position determined by a fraction
 * of the way from beginning to end. Ideally, this should be determined by
 * the number of displayable lines in the source. This routine does it as a
 * fraction of the first position and last position and then normalizes to
 * the start of the line containing the position.
 *
 * BUG/deficiency: The normalize to line portion of this routine will
 * cause thumbing to always position to the start of the source.
 */

/*ARGSUSED*/
static void 
VJump(w, closure, callData)
Widget w;
XtPointer closure, callData; /* closuer = TextWidget, callData = percent. */
{
  float * percent = (float *) callData;
  TextWidget ctx = (TextWidget)closure;
  XawTextPosition position, old_top, old_bot;
  XawTextLineTable * lt = &(ctx->text.lt);

  _XawTextPrepareToUpdate(ctx);
  old_top = lt->top;
  if ( (lt->lines > 0) && (IsValidLine(ctx, lt->lines - 1)) )
    old_bot = lt->info[lt->lines - 1].position;
  else
    old_bot = ctx->text.lastPos;

  position = (long) (*percent * (float) ctx->text.lastPos);
  position= SrcScan(ctx->text.source, position, XawstEOL, XawsdLeft, 1, FALSE);
  if ( (position >= old_top) && (position <= old_bot) ) {
    int line = 0;
    for (;(line < lt->lines) && (position > lt->info[line].position) ; line++);
    _XawTextVScroll(ctx, line);
  }
  else {
    XawTextPosition new_bot;
    _XawTextBuildLineTable(ctx, position, FALSE);
    new_bot = IsValidLine(ctx, lt->lines-1) ? lt->info[lt->lines-1].position 
                                            : ctx->text.lastPos;

    if ((old_top >= lt->top) && (old_top <= new_bot)) {
      int line = 0;
      for (;(line < lt->lines) && (old_top > lt->info[line].position); line++);
      _XawTextBuildLineTable(ctx, old_top, FALSE);
      _XawTextVScroll(ctx, -line);
    }
    else 
      DisplayTextWindow( (Widget) ctx);
  }
  _XawTextExecuteUpdate(ctx);
}

static Boolean
MatchSelection(selection, s)
Atom		    selection;
XawTextSelection    *s;
{
    Atom    *match;
    int	    count;

    for (count = 0, match = s->selections; count < s->atom_count; match++, count++)
	if (*match == selection)
	    return True;
    return False;
}

static Boolean 
ConvertSelection(w, selection, target, type, value, length, format)
Widget w;
Atom *selection, *target, *type;
XtPointer *value;
unsigned long *length;
int *format;
{
  Display* d = XtDisplay(w);
  TextWidget ctx = (TextWidget)w;
  Widget src = ctx->text.source;
  XawTextEditType edit_mode;
  Arg args[1];

  XawTextSelectionSalt	*salt = NULL;
  XawTextSelection	*s;

  if (*target == XA_TARGETS(d)) {
    Atom* targetP, * std_targets;
    unsigned long std_length;

    if ( SrcCvtSel(src, selection, target, type, value, length, format) ) 
	return True;

    XmuConvertStandardSelection(w, ctx->text.time, selection, 
				target, type, (XPointer*)&std_targets,
				&std_length, format);
    
    *value = XtMalloc((unsigned) sizeof(Atom)*(std_length + 7));
    targetP = *(Atom**)value;
    *length = std_length + 6;
    *targetP++ = XA_STRING;
    *targetP++ = XA_TEXT(d);
    *targetP++ = XA_COMPOUND_TEXT(d);
    *targetP++ = XA_LENGTH(d);
    *targetP++ = XA_LIST_LENGTH(d);
    *targetP++ = XA_CHARACTER_POSITION(d);

    XtSetArg(args[0], XtNeditType,&edit_mode);
    XtGetValues(src, args, ONE);

    if (edit_mode == XawtextEdit) {
      *targetP++ = XA_DELETE(d);
      (*length)++;
    }
    (void) memmove((char*)targetP, (char*)std_targets, sizeof(Atom)*std_length);
    XtFree((char*)std_targets);
    *type = XA_ATOM;
    *format = 32;
    return True;
  }
  
  if ( SrcCvtSel(src, selection, target, type, value, length, format) )
    return True;

  if (MatchSelection (*selection, &ctx->text.s))
    s = &ctx->text.s;
  else
  {
    for (salt = ctx->text.salt; salt; salt = salt->next)
	if (MatchSelection (*selection, &salt->s))
	    break;
    if (!salt)
	return False;
    s = &salt->s;
  }
  if (*target == XA_STRING ||
      *target == XA_TEXT(d) ||
      *target == XA_COMPOUND_TEXT(d)) {
	if (*target == XA_TEXT(d)) {
	    if (_XawTextFormat(ctx) == XawFmtWide)
		*type = XA_COMPOUND_TEXT(d);
	    else
		*type = XA_STRING;
	} else {
	    *type = *target;
	}
	/* 
	 * If salt is True, the salt->contents stores CT string, 
	 * its length is measured in bytes.
	 * Refer to _XawTextSaltAwaySelection().
	 *
	 * by Li Yuhong, Mar. 20, 1991.
	 */
	if (!salt) {
	    *value = _XawTextGetSTRING(ctx, s->left, s->right);
	    if (_XawTextFormat(ctx) == XawFmtWide) {
		XTextProperty textprop;
		if (XwcTextListToTextProperty(d, (wchar_t **)value, 1,
					      XCompoundTextStyle, &textprop)
			<  Success) {
		    XtFree(*value);
		    return False;
		}
		XtFree(*value);
		*value = (XtPointer)textprop.value;
		*length = textprop.nitems;
	    } else {
		*length = strlen(*value);
	    }
	} else {
	    *value = XtMalloc((salt->length + 1) * sizeof(unsigned char));
	    strcpy (*value, salt->contents);
	    *length = salt->length;
	}
	if (_XawTextFormat(ctx) == XawFmtWide && *type == XA_STRING) {
	    XTextProperty textprop;
	    wchar_t **wlist;
	    int count;
	    textprop.encoding = XA_COMPOUND_TEXT(d);
	    textprop.value = (unsigned char *)*value;
	    textprop.nitems = strlen(*value);
	    textprop.format = 8;
	    if (XwcTextPropertyToTextList(d, &textprop, (wchar_t ***)&wlist, &count)
			< Success) {
		XtFree(*value);
		return False;
	    }
	    XtFree(*value);
	    if (XwcTextListToTextProperty( d, (wchar_t **)wlist, 1,
					  XStringStyle, &textprop) < Success) {
		XwcFreeStringList( (wchar_t**) wlist );
		return False;
	    }
	    *value = (XtPointer) textprop.value;
	    *length = textprop.nitems;
	    XwcFreeStringList( (wchar_t**) wlist );
	}
	*format = 8;
	return True;
  }

  if ( (*target == XA_LIST_LENGTH(d)) || (*target == XA_LENGTH(d)) ) {
    long * temp;
    
    temp = (long *) XtMalloc( (unsigned) sizeof(long) );
    if (*target == XA_LIST_LENGTH(d))
      *temp = 1L;
    else			/* *target == XA_LENGTH(d) */
      *temp = (long) (s->right - s->left);
    
    *value = (XPointer) temp;
    *type = XA_INTEGER;
    *length = 1L;
    *format = 32;
    return True;
  }

  if (*target == XA_CHARACTER_POSITION(d)) {
    long * temp;
    
    temp = (long *) XtMalloc( (unsigned)( 2 * sizeof(long) ) );
    temp[0] = (long) (s->left + 1);
    temp[1] = s->right;
    *value = (XPointer) temp;
    *type = XA_SPAN(d);
    *length = 2L;
    *format = 32;
    return True;
  }

  if (*target == XA_DELETE(d)) {
    void _XawTextZapSelection(); /* From TextAction.c */
    
    if (!salt)
	_XawTextZapSelection( ctx, (XEvent *) NULL, TRUE);
    *value = NULL;
    *type = XA_NULL(d);
    *length = 0;
    *format = 32;
    return True;
  }

  if (XmuConvertStandardSelection(w, ctx->text.time, selection, target, type,
				  (XPointer *)value, length, format))
    return True;
  
  /* else */
  return False;
}

/*	Function Name: GetCutBuffferNumber
 *	Description: Returns the number of the cut buffer.
 *	Arguments: atom - the atom to check.
 *	Returns: the number of the cut buffer representing this atom or
 *               NOT_A_CUT_BUFFER.
 */

#define NOT_A_CUT_BUFFER -1

static int
GetCutBufferNumber(atom)
Atom atom;
{
  if (atom == XA_CUT_BUFFER0) return(0);
  if (atom == XA_CUT_BUFFER1) return(1);
  if (atom == XA_CUT_BUFFER2) return(2);
  if (atom == XA_CUT_BUFFER3) return(3);
  if (atom == XA_CUT_BUFFER4) return(4);
  if (atom == XA_CUT_BUFFER5) return(5);
  if (atom == XA_CUT_BUFFER6) return(6);
  if (atom == XA_CUT_BUFFER7) return(7);
  return(NOT_A_CUT_BUFFER);
}

static void 
LoseSelection(w, selection)
Widget w;
Atom *selection;
{
  TextWidget ctx = (TextWidget) w;
  Atom* atomP;
  int i;
  XawTextSelectionSalt	*salt, *prevSalt, *nextSalt;

  _XawTextPrepareToUpdate(ctx);

  atomP = ctx->text.s.selections;
  for (i = 0 ; i < ctx->text.s.atom_count; i++, atomP++)
    if ( (*selection == *atomP) || 
	(GetCutBufferNumber(*atomP) != NOT_A_CUT_BUFFER) )/* is a cut buffer */
      *atomP = (Atom)0;

  while (ctx->text.s.atom_count &&
	 ctx->text.s.selections[ctx->text.s.atom_count-1] == 0)
    ctx->text.s.atom_count--;

/*
 * Must walk the selection list in opposite order from UnsetSelection.
 */

  atomP = ctx->text.s.selections;
  for (i = 0 ; i < ctx->text.s.atom_count; i++, atomP++)
    if (*atomP == (Atom)0) {
      *atomP = ctx->text.s.selections[--ctx->text.s.atom_count];
      while (ctx->text.s.atom_count &&
	     ctx->text.s.selections[ctx->text.s.atom_count-1] == 0)
	ctx->text.s.atom_count--;
    }
  
  if (ctx->text.s.atom_count == 0)
    ModifySelection(ctx, ctx->text.insertPos, ctx->text.insertPos);
      
  if (ctx->text.old_insert >= 0) /* Update in progress. */
    _XawTextExecuteUpdate(ctx);

    prevSalt = 0;
    for (salt = ctx->text.salt; salt; salt = nextSalt)
    {
    	atomP = salt->s.selections;
	nextSalt = salt->next;
    	for (i = 0 ; i < salt->s.atom_count; i++, atomP++)
	    if (*selection == *atomP)
		*atomP = (Atom)0;
    	
    	while (salt->s.atom_count &&
	       salt->s.selections[salt->s.atom_count-1] == 0)
	{
	    salt->s.atom_count--;
	}
    	
    	/*
    	 * Must walk the selection list in opposite order from UnsetSelection.
    	 */
    	
    	atomP = salt->s.selections;
    	for (i = 0 ; i < salt->s.atom_count; i++, atomP++)
    	    if (*atomP == (Atom)0)
 	    {
      	      *atomP = salt->s.selections[--salt->s.atom_count];
      	      while (salt->s.atom_count &&
	     	     salt->s.selections[salt->s.atom_count-1] == 0)
    	    	salt->s.atom_count--;
    	    }
	if (salt->s.atom_count == 0)
	{
	    XtFree ((char *) salt->s.selections);
	    XtFree (salt->contents);
	    if (prevSalt)
		prevSalt->next = nextSalt;
	    else
		ctx->text.salt = nextSalt;
	    XtFree ((char *) salt);
	}
	else
	    prevSalt = salt;
    }
}

void
_XawTextSaltAwaySelection (ctx, selections, num_atoms)
TextWidget ctx;
Atom* selections;
int	num_atoms;
{
    XawTextSelectionSalt    *salt;
    int			    i, j;

    for (i = 0; i < num_atoms; i++)
	LoseSelection ((Widget) ctx, selections + i);
    if (num_atoms == 0)
	return;
    salt = (XawTextSelectionSalt *)
		XtMalloc( (unsigned) sizeof(XawTextSelectionSalt) );
    if (!salt)
	return;
    salt->s.selections = (Atom *)
	 XtMalloc( (unsigned) ( num_atoms * sizeof (Atom) ) );
    if (!salt->s.selections)
    {
	XtFree ((char *) salt);
	return;
    }
    salt->s.left = ctx->text.s.left;
    salt->s.right = ctx->text.s.right;
    salt->s.type = ctx->text.s.type;
    salt->contents = _XawTextGetSTRING(ctx, ctx->text.s.left, ctx->text.s.right);
    if (_XawTextFormat(ctx) == XawFmtWide) {
	XTextProperty textprop;
	if (XwcTextListToTextProperty(XtDisplay((Widget)ctx),
			(wchar_t**)(&(salt->contents)), 1, XCompoundTextStyle,
			&textprop) < Success) {
	    XtFree(salt->contents);
	    salt->length = 0;
	    return;
	}
	XtFree(salt->contents);
	salt->contents = (char *)textprop.value;
	salt->length = textprop.nitems;
    } else
       salt->length = strlen (salt->contents);
    salt->next = ctx->text.salt;
    ctx->text.salt = salt;
    j = 0;
    for (i = 0; i < num_atoms; i++)
    {
	if (GetCutBufferNumber (selections[i]) == NOT_A_CUT_BUFFER)
	{
	    salt->s.selections[j++] = selections[i];
	    XtOwnSelection ((Widget) ctx, selections[i], ctx->text.time,
		    ConvertSelection, LoseSelection, (XtSelectionDoneProc)NULL);
	}
    }
    salt->s.atom_count = j;
}

static void 
_SetSelection(ctx, left, right, selections, count)
TextWidget ctx;
XawTextPosition left, right;
Atom *selections;
Cardinal count;
{
  XawTextPosition pos;
  
  if (left < ctx->text.s.left) {
    pos = Min(right, ctx->text.s.left);
    _XawTextNeedsUpdating(ctx, left, pos);
  }
  if (left > ctx->text.s.left) {
    pos = Min(left, ctx->text.s.right);
    _XawTextNeedsUpdating(ctx, ctx->text.s.left, pos);
  }
  if (right < ctx->text.s.right) {
    pos = Max(right, ctx->text.s.left);
    _XawTextNeedsUpdating(ctx, pos, ctx->text.s.right);
  }
  if (right > ctx->text.s.right) {
    pos = Max(left, ctx->text.s.right);
    _XawTextNeedsUpdating(ctx, pos, right);
  }
  
  ctx->text.s.left = left;
  ctx->text.s.right = right;

  SrcSetSelection(ctx->text.source, left, right,
		  (count == 0) ? None : selections[0]);

  if (left < right) {
    Widget w = (Widget) ctx;
    int buffer;
    
    while (count) {
      Atom selection = selections[--count];

      if ((buffer = GetCutBufferNumber(selection)) != NOT_A_CUT_BUFFER) {

	unsigned char *ptr, *tptr;
	unsigned int amount, max_len = MAX_CUT_LEN(XtDisplay(w));
	unsigned long len;

	tptr= ptr= (unsigned char *) _XawTextGetSTRING(ctx, ctx->text.s.left, 
						       ctx->text.s.right);
	if (_XawTextFormat(ctx) == XawFmtWide) {
	   /*
	    * Only XA_STRING(Latin 1) is allowed in CUT_BUFFER,
	    * so we get it from wchar string, then free the wchar string.
	    */
	    XTextProperty textprop;
	    if (XwcTextListToTextProperty(XtDisplay(w), (wchar_t**)&ptr, 1,
		    XStringStyle, &textprop) <  Success) {
		XtFree((char *)ptr);
		return;
	    }
	    XtFree((char *)ptr);
	    tptr = ptr = textprop.value;
        } 
	if (buffer == 0) {
	  _CreateCutBuffers(XtDisplay(w));
	  XRotateBuffers(XtDisplay(w), 1);
	}
	amount = Min ( (len = strlen((char *)ptr)), max_len);
	XChangeProperty(XtDisplay(w), RootWindow(XtDisplay(w), 0), selection, 
			XA_STRING, 8, PropModeReplace, ptr, amount);

	while (len > max_len) {
	    len -= max_len;
	    tptr += max_len;
	    amount = Min (len, max_len);
	    XChangeProperty(XtDisplay(w), RootWindow(XtDisplay(w), 0), 
			    selection, XA_STRING, 8, PropModeAppend, 
			    tptr, amount);
	}
	XtFree ((char *)ptr);
      }
      else			/* This is a real selection */
      XtOwnSelection(w, selection, ctx->text.time, ConvertSelection, 
		     LoseSelection, (XtSelectionDoneProc)NULL);
    }
  }
  else
    XawTextUnsetSelection((Widget)ctx);
}

/*
 * This internal routine deletes the text from pos1 to pos2 in a source and
 * then inserts, at pos1, the text that was passed. As a side effect it
 * "invalidates" that portion of the displayed text (if any).
 *
 * NOTE: It is illegal to call this routine unless there is a valid line table!
 */

int 
_XawTextReplace (ctx, pos1, pos2, text)
TextWidget ctx;
XawTextPosition pos1, pos2;
XawTextBlock *text;
{
  int i, line1, delta, error;
  XawTextPosition updateFrom, updateTo;
  Widget src = ctx->text.source;
  XawTextEditType edit_mode;
  Arg args[1];
  Boolean tmp = ctx->text.update_disabled;

  ctx->text.update_disabled = True; /* No redisplay during replacement. */

/*
 * The insertPos may not always be set to the right spot in XawtextAppend 
 */

  XtSetArg(args[0], XtNeditType, &edit_mode);
  XtGetValues(src, args, ONE);

  if ((pos1 == ctx->text.insertPos) && (edit_mode == XawtextAppend)) {
    ctx->text.insertPos = ctx->text.lastPos;
    pos2 = SrcScan(src, ctx->text.insertPos, XawstPositions, XawsdRight,
		   (int)(ctx->text.insertPos - pos1), (Boolean)TRUE);
    pos1 = ctx->text.insertPos;
    if ( (pos1 == pos2) && (text->length == 0) ) {
      ctx->text.update_disabled = FALSE; /* rearm redisplay. */
      return( XawEditError );
    }
  }

  updateFrom = SrcScan(src, pos1, XawstWhiteSpace, XawsdLeft, 1, FALSE);
  updateFrom = Max(updateFrom, ctx->text.lt.top);

  line1 = LineForPosition(ctx, updateFrom);
  if ( (error = SrcReplace(src, pos1, pos2, text)) != 0) {
    ctx->text.update_disabled = tmp; /* restore redisplay */
    return(error);
  }

  XawTextUnsetSelection((Widget)ctx);

  ctx->text.lastPos = GETLASTPOS;
  if (ctx->text.lt.top >= ctx->text.lastPos) {
    _XawTextBuildLineTable(ctx, ctx->text.lastPos, FALSE);
    ClearWindow( (Widget) ctx);
    ctx->text.update_disabled = tmp; /* restore redisplay */
    return(0);			/* Things are fine. */
  }

  ctx->text.single_char = (text->length <= 1 && pos2 - pos1 <= 1);

  delta = text->length - (pos2 - pos1);

  if (delta < ctx->text.lastPos) {
    for (pos2 += delta, i = 0; i < ctx->text.numranges; i++) {
      if (ctx->text.updateFrom[i] > pos1)
	ctx->text.updateFrom[i] += delta;
      if (ctx->text.updateTo[i] >= pos1)
	ctx->text.updateTo[i] += delta;
    }
  }
  
  /* 
   * fixup all current line table entries to reflect edit.
   * %%% it is not legal to do arithmetic on positions.
   * using Scan would be more proper.
   */ 
  if (delta != 0) {
    XawTextLineTableEntry *lineP;
    i = LineForPosition(ctx, pos1) + 1;
    for (lineP = ctx->text.lt.info + i; i <= ctx->text.lt.lines; i++, lineP++)
      lineP->position += delta;
  }
  
  /*
   * Now process the line table and fixup in case edits caused
   * changes in line breaks. If we are breaking on word boundaries,
   * this code checks for moving words to and from lines.
   */
  
  if (IsPositionVisible(ctx, updateFrom)) {
    updateTo = _BuildLineTable(ctx, 
			       ctx->text.lt.info[line1].position, pos1, line1);
    _XawTextNeedsUpdating(ctx, updateFrom, updateTo);
  }

  ctx->text.update_disabled = tmp; /* restore redisplay */
  return(0);			/* Things are fine. */
}

/*
 * This routine will display text between two arbitrary source positions.
 * In the event that this span contains highlighted text for the selection, 
 * only that portion will be displayed highlighted.
 *
 * NOTE: it is illegal to call this routine unless there
 *       is a valid line table! 
 */

static void 
DisplayText(w, pos1, pos2)
Widget w;
XawTextPosition pos1, pos2;
{
  TextWidget ctx = (TextWidget)w;
  Position x, y;
  int height, line, i, lastPos = ctx->text.lastPos;
  XawTextPosition startPos, endPos;
  Boolean clear_eol, done_painting;

  pos1 = (pos1 < ctx->text.lt.top) ? ctx->text.lt.top : pos1;
  pos2 = FindGoodPosition(ctx, pos2);
  if ( (pos1 >= pos2) || !LineAndXYForPosition(ctx, pos1, &line, &x, &y) )
    return;			/* line not visible, or pos1 >= pos2. */

  for ( startPos = pos1, i = line; IsValidLine(ctx, i) && 
                                   (i < ctx->text.lt.lines) ; i++) {

    
    if ( (endPos = ctx->text.lt.info[i + 1].position) > pos2 ) {
      clear_eol = ((endPos = pos2) >= lastPos);
      done_painting = (!clear_eol || ctx->text.single_char);
    }
    else {
      clear_eol = TRUE;
      done_painting = FALSE;
    }

    height = ctx->text.lt.info[i + 1].y - ctx->text.lt.info[i].y;

    if ( (endPos > startPos) ) {
      if ( (x == (Position) ctx->text.margin.left) && (x > 0) )
	 SinkClearToBG (ctx->text.sink,
			(Position) 0, y, 
			(Dimension) ctx->text.margin.left, (Dimension)height); 

      if ( (startPos >= ctx->text.s.right) || (endPos <= ctx->text.s.left) ) 
	XawTextSinkDisplayText(ctx->text.sink, x, y, startPos, endPos, FALSE);
      else if ((startPos >= ctx->text.s.left) && (endPos <= ctx->text.s.right))
	XawTextSinkDisplayText(ctx->text.sink, x, y, startPos, endPos, TRUE);
      else {
	DisplayText(w, startPos, ctx->text.s.left);
	DisplayText(w, Max(startPos, ctx->text.s.left),
		    Min(endPos, ctx->text.s.right));
	DisplayText(w, ctx->text.s.right, endPos);
      }
    }
    startPos = endPos;
    if (clear_eol) {
	SinkClearToBG(ctx->text.sink, 
		      (Position) (ctx->text.lt.info[i].textWidth +
				  ctx->text.margin.left),
		      (Position) y, w->core.width, (Dimension) height);

	/*
	 * We only get here if single character is true, and we need
	 * to clear to the end of the screen.  We know that since there
	 * was only one character deleted that this is the same
	 * as clearing an extra line, so we do this, and are done.
	 * 
	 * This a performance hack, and a pretty gross one, but it works.
	 *
	 * Chris Peterson 11/13/89.
	 */

	if (done_painting) {
	    y += height;
	    SinkClearToBG(ctx->text.sink, 
			  (Position) ctx->text.margin.left, (Position) y, 
			  w->core.width, (Dimension) height);

	    break;		/* set single_char to FALSE and return. */
	}
    }

    x = (Position) ctx->text.margin.left;
    y = ctx->text.lt.info[i + 1].y;
    if ( done_painting
	 || (y >= (int)(ctx->core.height - ctx->text.margin.bottom)) )
      break;
  }
  ctx->text.single_char = FALSE;
}

/*
 * This routine implements multi-click selection in a hardwired manner.
 * It supports multi-click entity cycling (char, word, line, file) and mouse
 * motion adjustment of the selected entitie (i.e. select a word then, with
 * button still down, adjust wich word you really meant by moving the mouse).
 * [NOTE: This routine is to be replaced by a set of procedures that
 * will allows clients to implements a wide class of draw through and
 * multi-click selection user interfaces.]
 */

static void 
DoSelection (ctx, pos, time, motion)
TextWidget ctx;
XawTextPosition pos;
Time time;
Boolean motion;
{
  XawTextPosition newLeft, newRight;
  XawTextSelectType newType, *sarray;
  Widget src = ctx->text.source;

  if (motion)
    newType = ctx->text.s.type;
  else {
    if ( (abs((long) time - (long) ctx->text.lasttime) < MULTI_CLICK_TIME) &&
	 ((pos >= ctx->text.s.left) && (pos <= ctx->text.s.right))) {
      sarray = ctx->text.sarray;
      for (;*sarray != XawselectNull && *sarray != ctx->text.s.type; sarray++);

      if (*sarray == XawselectNull)
	newType = *(ctx->text.sarray);
      else {
	newType = *(sarray + 1);
	if (newType == XawselectNull) 
	  newType = *(ctx->text.sarray);
      }
    } 
    else 			                      /* single-click event */
      newType = *(ctx->text.sarray);

    ctx->text.lasttime = time;
  }
  switch (newType) {
  case XawselectPosition: 
    newLeft = newRight = pos;
    break;
  case XawselectChar: 
    newLeft = pos;
    newRight = SrcScan(src, pos, XawstPositions, XawsdRight, 1, FALSE);
    break;
  case XawselectWord: 
  case XawselectParagraph: 
    {
      XawTextScanType stype;

      if (newType == XawselectWord)
        stype = XawstWhiteSpace;
      else
	stype = XawstParagraph;

      /*
       * Somewhat complicated, but basically I treat the space between
       * two objects as another object.  The object that I am currently
       * in then becomes the end of the selection.
       * 
       * Chris Peterson - 4/19/90.
       */

      newRight = SrcScan(ctx->text.source, pos, stype, XawsdRight, 1, FALSE);
      newRight =SrcScan(ctx->text.source, newRight,stype,XawsdLeft,1, FALSE);

      if (pos != newRight) 
	newLeft = SrcScan(ctx->text.source, pos, stype, XawsdLeft, 1, FALSE);
      else
	newLeft = pos;

      newLeft =SrcScan(ctx->text.source, newLeft, stype, XawsdRight,1,FALSE);

      if (newLeft > newRight) {
	  XawTextPosition temp = newLeft;
	  newLeft = newRight;
	  newRight = temp;
      }
    }
    break;
  case XawselectLine: 
    newLeft = SrcScan(src, pos, XawstEOL, XawsdLeft, 1, FALSE);
    newRight = SrcScan(src, pos, XawstEOL, XawsdRight, 1, FALSE);
    break;
  case XawselectAll: 
    newLeft = SrcScan(src, pos, XawstAll, XawsdLeft, 1, FALSE);
    newRight = SrcScan(src, pos, XawstAll, XawsdRight, 1, FALSE);
    break;
  default:
    XtAppWarning(XtWidgetToApplicationContext((Widget) ctx),
	       "Text Widget: empty selection array.");
    return;
  }

  if ( (newLeft != ctx->text.s.left) || (newRight != ctx->text.s.right)
      || (newType != ctx->text.s.type)) {
    ModifySelection(ctx, newLeft, newRight);
    if (pos - ctx->text.s.left < ctx->text.s.right - pos)
      ctx->text.insertPos = newLeft;
    else 
      ctx->text.insertPos = newRight;
    ctx->text.s.type = newType;
  }
  if (!motion) { /* setup so we can freely mix select extend calls*/
    ctx->text.origSel.type = ctx->text.s.type;
    ctx->text.origSel.left = ctx->text.s.left;
    ctx->text.origSel.right = ctx->text.s.right;

    if (pos >= ctx->text.s.left + ((ctx->text.s.right - ctx->text.s.left) / 2))
      ctx->text.extendDir = XawsdRight;
    else
      ctx->text.extendDir = XawsdLeft;
  }
}

/*
 * This routine implements extension of the currently selected text in
 * the "current" mode (i.e. char word, line, etc.). It worries about
 * extending from either end of the selection and handles the case when you
 * cross through the "center" of the current selection (e.g. switch which
 * end you are extending!).
 */

static void 
ExtendSelection (ctx, pos, motion)
TextWidget ctx;
XawTextPosition pos;
Boolean motion;
{
  XawTextScanDirection dir;

  if (!motion) {		/* setup for extending selection */
    if (ctx->text.s.left == ctx->text.s.right) /* no current selection. */
      ctx->text.s.left = ctx->text.s.right = ctx->text.insertPos;   
    else {
      ctx->text.origSel.left = ctx->text.s.left;
      ctx->text.origSel.right = ctx->text.s.right;
    }
      
    ctx->text.origSel.type = ctx->text.s.type;

    if (pos >= ctx->text.s.left + ((ctx->text.s.right - ctx->text.s.left) / 2))
	ctx->text.extendDir = XawsdRight;
    else
	ctx->text.extendDir = XawsdLeft;
  }
  else /* check for change in extend direction */
    if ((ctx->text.extendDir == XawsdRight && pos <= ctx->text.origSel.left) ||
	(ctx->text.extendDir == XawsdLeft && pos >= ctx->text.origSel.right)) {
      ctx->text.extendDir = (ctx->text.extendDir == XawsdRight) ?
	                                            XawsdLeft : XawsdRight;
      ModifySelection(ctx, ctx->text.origSel.left, ctx->text.origSel.right);
    }

  dir = ctx->text.extendDir;
  switch (ctx->text.s.type) {
  case XawselectWord: 
  case XawselectParagraph:
    {
      XawTextPosition left_pos, right_pos;
      XawTextScanType stype;

      if (ctx->text.s.type == XawselectWord)
        stype = XawstWhiteSpace;
      else
	stype = XawstParagraph;

      /*
       * Somewhat complicated, but basically I treat the space between
       * two objects as another object.  The object that I am currently
       * in then becomes the end of the selection.
       * 
       * Chris Peterson - 4/19/90.
       */

      right_pos = SrcScan(ctx->text.source, pos, stype, XawsdRight, 1, FALSE);
      right_pos =SrcScan(ctx->text.source, right_pos,stype,XawsdLeft,1, FALSE);

      if (pos != right_pos) 
	left_pos = SrcScan(ctx->text.source, pos, stype, XawsdLeft, 1, FALSE);
      else
	left_pos = pos;

      left_pos =SrcScan(ctx->text.source, left_pos, stype, XawsdRight,1,FALSE);

      if (dir == XawsdLeft)
	pos = Min(left_pos, right_pos);
      else /* dir == XawsdRight */
	pos = Max(left_pos, right_pos);
    }
    break;
  case XawselectLine:
    pos = SrcScan(ctx->text.source, pos, XawstEOL, dir, 1, dir == XawsdRight);
    break;
  case XawselectAll: 
    pos = ctx->text.insertPos;
  case XawselectPosition:	/* fall through. */
  default:
    break;
  }
  
  if (dir == XawsdRight)
    ModifySelection(ctx, ctx->text.s.left, pos);
  else
    ModifySelection(ctx, pos, ctx->text.s.right);

  ctx->text.insertPos = pos;
}

/*
 * Clear the window to background color.
 */

static void
ClearWindow (w)
Widget w;
{
  TextWidget ctx = (TextWidget) w;

  if (XtIsRealized(w))
    SinkClearToBG(ctx->text.sink, 
		  (Position) 0, (Position) 0, 
		  w->core.width, w->core.height);
}

/*	Function Name: _XawTextClearAndCenterDisplay
 *	Description: Redraws the display with the cursor in insert point
 *                   centered vertically.
 *	Arguments: ctx - the text widget.
 *	Returns: none.
 */

void
_XawTextClearAndCenterDisplay(ctx)
TextWidget ctx;
{
  int insert_line = LineForPosition(ctx, ctx->text.insertPos);
  int scroll_by = insert_line - ctx->text.lt.lines/2;

  _XawTextVScroll(ctx, scroll_by);
  DisplayTextWindow( (Widget) ctx);
}
  
/*
 * Internal redisplay entire window.
 * Legal to call only if widget is realized.
 */

static void
DisplayTextWindow (w)
Widget w;
{
  TextWidget ctx = (TextWidget) w;
  ClearWindow(w);
  _XawTextBuildLineTable(ctx, ctx->text.lt.top, FALSE);
  _XawTextNeedsUpdating(ctx, zeroPosition, ctx->text.lastPos);
  _XawTextSetScrollBars(ctx);
}

/*
 * This routine checks to see if the window should be resized (grown or
 * shrunk) when text to be painted overflows to the right or
 * the bottom of the window. It is used by the keyboard input routine.
 */

void
_XawTextCheckResize(ctx)
TextWidget ctx;
{
  Widget w = (Widget) ctx;
  int line = 0, old_height;
  XtWidgetGeometry rbox, return_geom;

  if ( (ctx->text.resize == XawtextResizeWidth) ||
       (ctx->text.resize == XawtextResizeBoth) ) {
    XawTextLineTableEntry *lt;
    rbox.width = 0;
    for (lt = ctx->text.lt.info; 
	 IsValidLine(ctx, line) && (line < ctx->text.lt.lines);
	 line++, lt++) {
      if ((int)(lt->textWidth + ctx->text.margin.left) > (int)rbox.width)
	  rbox.width = lt->textWidth + ctx->text.margin.left;
    }
    
    rbox.width += ctx->text.margin.right;
    if (rbox.width > ctx->core.width) { /* Only get wider. */
      rbox.request_mode = CWWidth;
      if (XtMakeGeometryRequest(w, &rbox, &return_geom) == XtGeometryAlmost)
	(void) XtMakeGeometryRequest(w, &return_geom, (XtWidgetGeometry*) NULL);
    }
  }

  if ( !((ctx->text.resize == XawtextResizeHeight) ||
	 (ctx->text.resize == XawtextResizeBoth)) )
      return;

  if (IsPositionVisible(ctx, ctx->text.lastPos))
    line = LineForPosition(ctx, ctx->text.lastPos);
  else
    line = ctx->text.lt.lines;
  
  if ( (line + 1) == ctx->text.lt.lines ) return;
  
  old_height = ctx->core.height;
  rbox.request_mode = CWHeight;
  rbox.height = XawTextSinkMaxHeight(ctx->text.sink, line + 1) + VMargins(ctx);
  
  if ((int)rbox.height < old_height) return; /* It will only get taller. */

  if (XtMakeGeometryRequest(w, &rbox, &return_geom) == XtGeometryAlmost)
    if (XtMakeGeometryRequest(w, &return_geom, (XtWidgetGeometry*)NULL) != XtGeometryYes)
      return;
  
  _XawTextBuildLineTable(ctx, ctx->text.lt.top, TRUE);
}

/*
 * Converts (params, num_params) to a list of atoms & caches the
 * list in the TextWidget instance.
 */

Atom*
_XawTextSelectionList(ctx, list, nelems)
TextWidget ctx;
String *list;
Cardinal nelems;
{
  Atom * sel = ctx->text.s.selections;
  Display *dpy = XtDisplay((Widget) ctx);
  int n;

  if (nelems > ctx->text.s.array_size) {
    sel = (Atom *) XtRealloc((char *) sel, sizeof(Atom) * nelems);
    ctx->text.s.array_size = nelems;
    ctx->text.s.selections = sel;
  }
  for (n=nelems; --n >= 0; sel++, list++)
    *sel = XInternAtom(dpy, *list, False);

  ctx->text.s.atom_count = nelems;
  return ctx->text.s.selections;
}

/*	Function Name: SetSelection
 *	Description: Sets the current selection.
 *	Arguments: ctx - the text widget.
 *                 defaultSel - the default selection.
 *                 l, r - the left and right ends of the selection.
 *                 list, nelems - the selection list (as strings).
 *	Returns: none.
 *
 *  NOTE: if (ctx->text.s.left >= ctx->text.s.right) then the selection
 *        is unset.
 */

void
_XawTextSetSelection(ctx, l, r, list, nelems)
TextWidget ctx;
XawTextPosition l, r;
String *list;
Cardinal nelems;
{
  if (nelems == 1 && !strcmp (list[0], "none"))
    return;
  if (nelems == 0) {
    String defaultSel = "PRIMARY";
    list = &defaultSel;
    nelems = 1;
  }
  _SetSelection(ctx, l, r, _XawTextSelectionList(ctx, list, nelems), nelems);
}


/*	Function Name: ModifySelection
 *	Description: Modifies the current selection.
 *	Arguments: ctx - the text widget.
 *                 left, right - the left and right ends of the selection.
 *	Returns: none.
 *
 *  NOTE: if (ctx->text.s.left >= ctx->text.s.right) then the selection
 *        is unset.
 */

static void
ModifySelection(ctx, left, right)
TextWidget ctx;
XawTextPosition left, right;
{
  if (left == right) 
    ctx->text.insertPos = left;
  _SetSelection( ctx, left, right, (Atom*) NULL, ZERO );
}

/*
 * This routine is used to perform various selection functions. The goal is
 * to be able to specify all the more popular forms of draw-through and
 * multi-click selection user interfaces from the outside.
 */

void 
_XawTextAlterSelection (ctx, mode, action, params, num_params)
TextWidget ctx;
XawTextSelectionMode mode;   /* {XawsmTextSelect, XawsmTextExtend} */
XawTextSelectionAction action; /* {XawactionStart, 
				  XawactionAdjust, XawactionEnd} */
String	*params;
Cardinal	*num_params;
{
  XawTextPosition position;
  Boolean flag;

/*
 * This flag is used by TextPop.c:DoReplace() to determine if the selection
 * is okay to use, or if it has been modified.
 */
    
  if (ctx->text.search != NULL)
    ctx->text.search->selection_changed = TRUE;

  position = PositionForXY (ctx, (int) ctx->text.ev_x, (int) ctx->text.ev_y);

  flag = (action != XawactionStart);
  if (mode == XawsmTextSelect)
    DoSelection (ctx, position, ctx->text.time, flag);
  else			/* mode == XawsmTextExtend */
   ExtendSelection (ctx, position, flag);

  if (action == XawactionEnd) 
    _XawTextSetSelection(ctx, ctx->text.s.left, ctx->text.s.right,
			 params, *num_params);
}

/*	Function Name: RectanglesOverlap
 *	Description: Returns TRUE if two rectangles overlap.
 *	Arguments: rect1, rect2 - the two rectangles to check.
 *	Returns: TRUE iff these rectangles overlap.
 */

static Boolean
RectanglesOverlap(rect1, rect2)
XRectangle *rect1, *rect2;
{
  return ( (rect1->x < rect2->x + (short) rect2->width) &&
	   (rect2->x < rect1->x + (short) rect1->width) &&
	   (rect1->y < rect2->y + (short) rect2->height) &&
	   (rect2->y < rect1->y + (short) rect1->height) );
}

/*	Function Name: UpdateTextInRectangle.
 *	Description: Updates the text in a rectangle.
 *	Arguments: ctx - the text widget.
 *                 rect - the rectangle to update.
 *	Returns: none.
 */

static void
UpdateTextInRectangle(ctx, rect)
TextWidget ctx;
XRectangle * rect;
{
  XawTextLineTableEntry *info = ctx->text.lt.info;
  int line, x = rect->x, y = rect->y;
  int right = rect->width + x, bottom = rect->height + y;

  for (line = 0;( (line < ctx->text.lt.lines) &&
		 IsValidLine(ctx, line) && (info->y < bottom)); line++, info++)
    if ( (info + 1)->y >= y ) 
      UpdateTextInLine(ctx, line, x, right);
}

/*
 * This routine processes all "expose region" XEvents. In general, its job
 * is to the best job at minimal re-paint of the text, displayed in the
 * window, that it can.
 */

/* ARGSUSED */
static void
ProcessExposeRegion(w, event, region)
Widget w;
XEvent *event;
Region region;			/* Unused. */
{
    TextWidget ctx = (TextWidget) w;
    XRectangle expose, cursor;
    Boolean need_to_draw;
    
    if (event->type == Expose) {
	expose.x = event->xexpose.x;
	expose.y = event->xexpose.y;
	expose.width = event->xexpose.width;
	expose.height = event->xexpose.height;
    }
    else if (event->type == GraphicsExpose) {
	expose.x = event->xgraphicsexpose.x;
	expose.y = event->xgraphicsexpose.y;
	expose.width = event->xgraphicsexpose.width;
	expose.height = event->xgraphicsexpose.height;
    }      
    else { /* No Expose */
	PopCopyQueue(ctx);		
	return;			/* no more processing necessary. */
    }

    need_to_draw = TranslateExposeRegion(ctx, &expose);
    if ((event->type == GraphicsExpose) && (event->xgraphicsexpose.count == 0))
	PopCopyQueue(ctx);	

    if (!need_to_draw) 
	return;			/* don't draw if we don't need to. */

    _XawTextPrepareToUpdate(ctx);
    UpdateTextInRectangle(ctx, &expose);
    XawTextSinkGetCursorBounds(ctx->text.sink, &cursor);
    if (RectanglesOverlap(&cursor, &expose)) {
	SinkClearToBG(ctx->text.sink, (Position) cursor.x, (Position) cursor.y,
		      (Dimension) cursor.width, (Dimension) cursor.height);
	UpdateTextInRectangle(ctx, &cursor);
    }
    _XawTextExecuteUpdate(ctx);
}

/*
 * This routine does all setup required to syncronize batched screen updates
 */

void
_XawTextPrepareToUpdate(ctx)
TextWidget ctx;
{
  if (ctx->text.old_insert < 0) {
    InsertCursor((Widget)ctx, XawisOff);
    ctx->text.numranges = 0;
    ctx->text.showposition = FALSE;
    ctx->text.old_insert = ctx->text.insertPos;
  }
}

/*
 * This is a private utility routine used by _XawTextExecuteUpdate. It
 * processes all the outstanding update requests and merges update
 * ranges where possible.
 */

static 
void FlushUpdate(ctx)
TextWidget ctx;
{
  int i, w;
  XawTextPosition updateFrom, updateTo;
  if (!XtIsRealized((Widget)ctx)) {
    ctx->text.numranges = 0;
    return;
  }
  while (ctx->text.numranges > 0) {
    updateFrom = ctx->text.updateFrom[0];
    w = 0;
    for (i = 1 ; i < ctx->text.numranges ; i++) {
      if (ctx->text.updateFrom[i] < updateFrom) {
	updateFrom = ctx->text.updateFrom[i];
	w = i;
      }
    }
    updateTo = ctx->text.updateTo[w];
    ctx->text.numranges--;
    ctx->text.updateFrom[w] = ctx->text.updateFrom[ctx->text.numranges];
    ctx->text.updateTo[w] = ctx->text.updateTo[ctx->text.numranges];
    for (i = ctx->text.numranges - 1 ; i >= 0 ; i--) {
      while (ctx->text.updateFrom[i] <= updateTo && i < ctx->text.numranges) {
	updateTo = ctx->text.updateTo[i];
	ctx->text.numranges--;
	ctx->text.updateFrom[i] = ctx->text.updateFrom[ctx->text.numranges];
	ctx->text.updateTo[i] = ctx->text.updateTo[ctx->text.numranges];
      }
    }
    DisplayText((Widget)ctx, updateFrom, updateTo);
  }
}

/*
 * This is a private utility routine used by _XawTextExecuteUpdate. This
 * routine worries about edits causing new data or the insertion point becoming
 * invisible (off the screen, or under the horiz. scrollbar). Currently 
 * it always makes it visible by scrolling. It probably needs 
 * generalization to allow more options.
 */

void
_XawTextShowPosition(ctx)
TextWidget ctx;
{
  int x, y, lines, number;
  Boolean no_scroll;
  XawTextPosition max_pos, top, first;

  if ( (!XtIsRealized((Widget)ctx)) || (ctx->text.lt.lines <= 0) )
    return;

/*
 * Find out the bottom the visable window, and make sure that the
 * cursor does not go past the end of this space.  
 *
 * This makes sure that the cursor does not go past the end of the 
 * visable window. 
 */

  x = ctx->core.width;
  y = ctx->core.height - ctx->text.margin.bottom;
  if (ctx->text.hbar != NULL)
    y -= ctx->text.hbar->core.height + 2 * ctx->text.hbar->core.border_width;
  
  max_pos = PositionForXY (ctx, x, y);
  lines = LineForPosition(ctx, max_pos) + 1; /* number of visable lines. */
  
  if ( (ctx->text.insertPos >= ctx->text.lt.top) &&
       (ctx->text.insertPos < max_pos))
    return;

  first = ctx->text.lt.top;
  no_scroll = FALSE;

  if (ctx->text.insertPos < first) { /* We need to scroll down. */
      top = SrcScan(ctx->text.source, ctx->text.insertPos,
		    XawstEOL, XawsdLeft, 1, FALSE);

      /* count the number of lines we have to scroll */

      number = 0;
      while (first > top) {
	  first = SrcScan(ctx->text.source, first,
			  XawstEOL, XawsdLeft, 1, TRUE);

	  if ( - number > lines ) 
	      break;

	  number--;
      }

      if (first <= top) {	/* If we found the proper number
				   of lines. */
     
	  /* Back up to just before the last CR. */
      
	  first = SrcScan(ctx->text.source, first,
			  XawstPositions, XawsdRight, 1, TRUE);
	  
	  /* Check to make sure the cursor is visable. */
	  
	  if (first <= top)
	      number++;
	  
	  lines = number;
      }
      else
	  no_scroll = TRUE;
  }
  else {			/* We need to Scroll up */
      top = SrcScan(ctx->text.source, ctx->text.insertPos,
		    XawstEOL, XawsdLeft, lines, FALSE);

      if (top < max_pos) 
	  lines = LineForPosition(ctx, top);
      else 
	  no_scroll = TRUE;
  }

  if (no_scroll) {
      _XawTextBuildLineTable(ctx, top, FALSE);      
      DisplayTextWindow((Widget)ctx);
  }
  else 
      _XawTextVScroll(ctx, lines);

  _XawTextSetScrollBars(ctx);
}

/*
 * This routine causes all batched screen updates to be performed
 */

void
_XawTextExecuteUpdate(ctx)
TextWidget ctx;
{
  if ( ctx->text.update_disabled || (ctx->text.old_insert < 0) ) 
    return;

  if((ctx->text.old_insert != ctx->text.insertPos) || (ctx->text.showposition))
    _XawTextShowPosition(ctx);
  FlushUpdate(ctx);
  InsertCursor((Widget)ctx, XawisOn);
  ctx->text.old_insert = -1;
}


static void 
TextDestroy(w)
Widget w;
{
  TextWidget ctx = (TextWidget)w;

  DestroyHScrollBar(ctx);
  DestroyVScrollBar(ctx);

  XtFree((char *)ctx->text.s.selections);
  XtFree((char *)ctx->text.lt.info);
  XtFree((char *)ctx->text.search);
  XtFree((char *)ctx->text.updateFrom);
  XtFree((char *)ctx->text.updateTo);
}

/*
 * by the time we are managed (and get this far) we had better
 * have both a source and a sink 
 */

static void
Resize(w)
Widget w;
{
  TextWidget ctx = (TextWidget) w;

  PositionVScrollBar(ctx);
  PositionHScrollBar(ctx);

  _XawTextBuildLineTable(ctx, ctx->text.lt.top, TRUE);
  _XawTextSetScrollBars(ctx);
}

/*
 * This routine allow the application program to Set attributes.
 */

/*ARGSUSED*/
static Boolean 
SetValues(current, request, new, args, num_args)
Widget current, request, new;
ArgList args;
Cardinal *num_args;
{
  TextWidget oldtw = (TextWidget) current;
  TextWidget newtw = (TextWidget) new;
  Boolean    redisplay = FALSE;
  Boolean    display_caret = newtw->text.display_caret;


  newtw->text.display_caret = oldtw->text.display_caret;
  _XawTextPrepareToUpdate(newtw);
  newtw->text.display_caret = display_caret;

  if (oldtw->text.r_margin.left != newtw->text.r_margin.left) {
    newtw->text.margin.left = newtw->text.r_margin.left;
    if (newtw->text.vbar != NULL)
      newtw->text.margin.left += newtw->text.vbar->core.width +
	                         newtw->text.vbar->core.border_width;
    redisplay = TRUE;
  }
  
  if (oldtw->text.scroll_vert != newtw->text.scroll_vert) {
    if (newtw->text.scroll_vert == XawtextScrollNever) 
      DestroyVScrollBar(newtw);
    else if (newtw->text.scroll_vert == XawtextScrollAlways)
      CreateVScrollBar(newtw);
    redisplay = TRUE;
  }

  if (oldtw->text.r_margin.bottom != newtw->text.r_margin.bottom) {
    newtw->text.margin.bottom = newtw->text.r_margin.bottom;
    if (newtw->text.hbar != NULL)
      newtw->text.margin.bottom += newtw->text.hbar->core.height +
	                           newtw->text.hbar->core.border_width;
    redisplay = TRUE;
  }
  
  if (oldtw->text.scroll_horiz != newtw->text.scroll_horiz) {
    if (newtw->text.scroll_horiz == XawtextScrollNever) 
      DestroyHScrollBar(newtw);
    else if (newtw->text.scroll_horiz == XawtextScrollAlways)
      CreateHScrollBar(newtw);
    redisplay = TRUE;
  }

  if ( oldtw->text.source != newtw->text.source )
    XawTextSetSource( (Widget) newtw, newtw->text.source, newtw->text.lt.top);

  newtw->text.redisplay_needed = False;
  XtSetValues( (Widget)newtw->text.source, args, *num_args );
  XtSetValues( (Widget)newtw->text.sink, args, *num_args );

  if ( oldtw->text.wrap != newtw->text.wrap ||
       oldtw->text.lt.top != newtw->text.lt.top ||
       oldtw->text.r_margin.right != newtw->text.r_margin.right ||
       oldtw->text.r_margin.top != newtw->text.r_margin.top ||
       oldtw->text.sink != newtw->text.sink ||
       newtw->text.redisplay_needed )
  {
    _XawTextBuildLineTable(newtw, newtw->text.lt.top, TRUE);
    redisplay = TRUE;
  }

  if (oldtw->text.insertPos != newtw->text.insertPos) {
    newtw->text.showposition = TRUE;
    redisplay = TRUE;
  }

  _XawTextExecuteUpdate(newtw);
  if (redisplay)
    _XawTextSetScrollBars(newtw);

  return redisplay;
}

/* invoked by the Simple widget's SetValues */
static Boolean ChangeSensitive(w)
    Widget w;	/* the new widget */
{
    Arg args[1];
    TextWidget tw = (TextWidget) w;

    (*(&simpleClassRec)->simple_class.change_sensitive)(w);

    XtSetArg(args[0], XtNancestorSensitive, 
	       (tw->core.ancestor_sensitive && tw->core.sensitive));
    if (tw->text.vbar)
	XtSetValues(tw->text.vbar, args, ONE);
    if (tw->text.hbar)
	XtSetValues(tw->text.hbar, args, ONE);
    return False;
}

/*	Function Name: GetValuesHook
 *	Description: This is a get values hook routine that gets the
 *                   values in the text source and sink.
 *	Arguments: w - the Text Widget.
 *                 args - the argument list.
 *                 num_args - the number of args.
 *	Returns: none.
 */

static void
GetValuesHook(w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
{
  XtGetValues( ((TextWidget) w)->text.source, args, *num_args );
  XtGetValues( ((TextWidget) w)->text.sink, args, *num_args );
}

/*	Function Name: FindGoodPosition
 *	Description: Returns a valid position given any postition
 *	Arguments: pos - any position.
 *	Returns: a position between (0 and lastPos);
 */

static XawTextPosition
FindGoodPosition(ctx, pos)
TextWidget ctx;
XawTextPosition pos;
{
  if (pos < 0) return(0);
  return ( ((pos > ctx->text.lastPos) ? ctx->text.lastPos : pos) );
}

/************************************************************
 * 
 * Routines for handling the copy area expose queue.
 *
 ************************************************************/

/*	Function Name: PushCopyQueue
 *	Description: Pushes a value onto the copy queue.
 *	Arguments: ctx - the text widget.
 *                 h, v - amount of offset in the horiz and vert directions.
 *	Returns: none
 */

static void
PushCopyQueue(ctx, h, v)
TextWidget ctx;
int h, v;
{
    struct text_move * offsets = XtNew(struct text_move);

    offsets->h = h;
    offsets->v = v;
    offsets->next = NULL;

    if (ctx->text.copy_area_offsets == NULL)
	ctx->text.copy_area_offsets = offsets;
    else {
	struct text_move * end = ctx->text.copy_area_offsets;
	for ( ; end->next != NULL; end = end->next) {}
	end->next = offsets;
    }
}

/*	Function Name: PopCopyQueue
 *	Description: Pops the top value off of the copy queue.
 *	Arguments: ctx - the text widget.
 *	Returns: none.
 */

static void
PopCopyQueue(ctx)
TextWidget ctx;
{
    struct text_move * offsets = ctx->text.copy_area_offsets;

    if (offsets == NULL)
	(void) printf( "Xaw Text widget %s: empty copy queue\n",
		       XtName( (Widget) ctx ) );
    else {
	ctx->text.copy_area_offsets = offsets->next;
	XtFree((char *) offsets);	/* free what you allocate. */
    }
}

/*	Function Name:  TranslateExposeRegion
 *	Description: Translates the expose that came into
 *                   the cordinates that now exist in the Text widget.
 *	Arguments: ctx - the text widget.
 *                 expose - a Rectangle, who's region currently
 *                          contains the expose event location.
 *                          this region will be returned containing
 *                          the new rectangle.
 *	Returns: True if there is drawing that needs to be done.
 */

static Boolean
TranslateExposeRegion(ctx, expose)
TextWidget ctx;
XRectangle * expose;
{
    struct text_move * offsets = ctx->text.copy_area_offsets;
    int value;
    int x, y, width, height;

    /*
     * Skip over the first one, this has already been taken into account.
     */

    if (!offsets || !(offsets = offsets->next))
	return(TRUE);

    x = expose->x;
    y = expose->y;
    width = expose->width;
    height = expose->height;

    while (offsets) {
	x += offsets->h;
	y += offsets->v;
	offsets = offsets->next;
    }

    /*
     * remove that area of the region that is now outside the window.
     */

    if (y < 0) {
	height += y;
	y = 0;
    }

    value = y + height - ctx->core.height;
    if (value > 0)
	height -= value;

    if (height <= 0)
	return(FALSE);		/* no need to draw outside the window. */

    /*
     * and now in the horiz direction...
     */

    if (x < 0) {
	width += x;
	x = 0;
    }

    value = x + width - ctx->core.width;
    if (value > 0)
	width -= value;

    if (width <= 0)
	return(FALSE);		/* no need to draw outside the window. */
    
    expose->x = x;
    expose->y = y;
    expose->width = width;
    expose->height = height;
    return(TRUE);
}

/* Li wrote this so the IM can find a given text position's screen position. */

void
#if NeedFunctionPrototypes
_XawTextPosToXY( 
    Widget w, 
    XawTextPosition pos, 
    Position* x, 
    Position* y )
#else
_XawTextPosToXY( w, pos, x, y )
    Widget w;
    XawTextPosition pos;
    Position *x, *y;
#endif
{
    int line;
    LineAndXYForPosition( (TextWidget)w, pos, &line, x, y );
}

/*******************************************************************
The following routines provide procedural interfaces to Text window state
setting and getting. They need to be redone so than the args code can use
them. I suggest we create a complete set that takes the context as an
argument and then have the public version lookup the context and call the
internal one. The major value of this set is that they have actual application
clients and therefore the functionality provided is required for any future
version of Text.
********************************************************************/

void
#if NeedFunctionPrototypes
XawTextDisplay (Widget w)
#else
XawTextDisplay (w)
Widget w;
#endif
{
  if (!XtIsRealized(w)) return;
  
  _XawTextPrepareToUpdate( (TextWidget) w);
  DisplayTextWindow(w);
  _XawTextExecuteUpdate( (TextWidget) w);
}

void
#if NeedFunctionPrototypes
XawTextSetSelectionArray(Widget w, XawTextSelectType *sarray)
#else
XawTextSetSelectionArray(w, sarray)
Widget w;
XawTextSelectType *sarray;
#endif
{
  ((TextWidget)w)->text.sarray = sarray;
}

void
#if NeedFunctionPrototypes
XawTextGetSelectionPos(Widget w, XawTextPosition *left, XawTextPosition *right)
#else
XawTextGetSelectionPos(w, left, right)
Widget w;
XawTextPosition* left,* right;
#endif
{
  *left = ((TextWidget) w)->text.s.left;
  *right = ((TextWidget) w)->text.s.right;
}


void 
#if NeedFunctionPrototypes
XawTextSetSource(Widget w, Widget source, XawTextPosition startPos)
#else
XawTextSetSource(w, source, startPos)
Widget w, source;
XawTextPosition startPos;
#endif
{
  TextWidget ctx = (TextWidget) w;

  ctx->text.source = source;
  ctx->text.lt.top = startPos;
  ctx->text.s.left = ctx->text.s.right = 0;
  ctx->text.insertPos = startPos;
  ctx->text.lastPos = GETLASTPOS;
  
  _XawTextBuildLineTable(ctx, ctx->text.lt.top, TRUE);
  XawTextDisplay(w);
}

/*
 * This public routine deletes the text from startPos to endPos in a source and
 * then inserts, at startPos, the text that was passed. As a side effect it
 * "invalidates" that portion of the displayed text (if any), so that things
 * will be repainted properly.
 */

int 
#if NeedFunctionPrototypes
XawTextReplace(Widget w, XawTextPosition startPos, XawTextPosition endPos,
               XawTextBlock *text)
#else
XawTextReplace(w, startPos, endPos, text)
Widget w;
XawTextPosition  startPos, endPos;
XawTextBlock *text;
#endif
{
  TextWidget ctx = (TextWidget) w;
  int result;

  _XawTextPrepareToUpdate(ctx);
  endPos = FindGoodPosition(ctx, endPos);
  startPos = FindGoodPosition(ctx, startPos);
  if ((result = _XawTextReplace(ctx, startPos, endPos, text)) == XawEditDone) {
    int delta = text->length - (endPos - startPos);
    if (ctx->text.insertPos >= (endPos + delta)) {
      XawTextScanDirection sd = (delta < 0) ? XawsdLeft : XawsdRight;
      ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos,
				    XawstPositions, sd, abs(delta), TRUE);
    }
  }

  _XawTextCheckResize(ctx);
  _XawTextExecuteUpdate(ctx);
  _XawTextSetScrollBars(ctx);
  
  return result;
}

XawTextPosition 
#if NeedFunctionPrototypes
XawTextTopPosition(Widget w)
#else
XawTextTopPosition(w)
Widget w;
#endif
{
  return( ((TextWidget) w)->text.lt.top );
}

void 
#if NeedFunctionPrototypes
XawTextSetInsertionPoint(Widget w, XawTextPosition position)
#else
XawTextSetInsertionPoint(w, position)
Widget w;
XawTextPosition position;
#endif
{
  TextWidget ctx = (TextWidget) w;

  _XawTextPrepareToUpdate(ctx);
  ctx->text.insertPos = FindGoodPosition(ctx, position);
  ctx->text.showposition = TRUE;

  _XawTextExecuteUpdate(ctx);
}

XawTextPosition
#if NeedFunctionPrototypes
XawTextGetInsertionPoint(Widget w)
#else
XawTextGetInsertionPoint(w)
Widget w;
#endif
{
  return( ((TextWidget) w)->text.insertPos);
}

/*
 * NOTE: Must walk the selection list in opposite order from LoseSelection.
 */

void 
#if NeedFunctionPrototypes
XawTextUnsetSelection(Widget w)
#else
XawTextUnsetSelection(w)
Widget w;
#endif
{
  TextWidget ctx = (TextWidget)w;

  while (ctx->text.s.atom_count != 0) {
    Atom sel = ctx->text.s.selections[ctx->text.s.atom_count - 1];
    if ( sel != (Atom) 0 ) {
/*
 * As selections are lost the atom_count will decrement.
 */
      if (GetCutBufferNumber(sel) == NOT_A_CUT_BUFFER)
	XtDisownSelection(w, sel, ctx->text.time);
      LoseSelection(w, &sel); /* In case this is a cut buffer, or 
				 XtDisownSelection failed to call us. */
    }
  }
}

void
#if NeedFunctionPrototypes
XawTextSetSelection (Widget w, XawTextPosition left, XawTextPosition right)
#else
XawTextSetSelection (w, left, right)
Widget w;
XawTextPosition left, right;
#endif
{
  TextWidget ctx = (TextWidget) w;
  
  _XawTextPrepareToUpdate(ctx);
  _XawTextSetSelection(ctx, FindGoodPosition(ctx, left),
		       FindGoodPosition(ctx, right), (String*)NULL, ZERO);
  _XawTextExecuteUpdate(ctx);
}

void 
#if NeedFunctionPrototypes
XawTextInvalidate(Widget w, XawTextPosition from, XawTextPosition to)
#else
XawTextInvalidate(w, from, to)
Widget w;
XawTextPosition from,to;
#endif
{
  TextWidget ctx = (TextWidget) w;

  from = FindGoodPosition(ctx, from);
  to = FindGoodPosition(ctx, to);
  ctx->text.lastPos = GETLASTPOS;
  _XawTextPrepareToUpdate(ctx);
  _XawTextNeedsUpdating(ctx, from, to);
  _XawTextBuildLineTable(ctx, ctx->text.lt.top, TRUE);
  _XawTextExecuteUpdate(ctx);
}

/*ARGSUSED*/
void
#if NeedFunctionPrototypes
XawTextDisableRedisplay(Widget w)
#else
XawTextDisableRedisplay(w)
Widget w;
#endif
{
  ((TextWidget) w)->text.update_disabled = True;
  _XawTextPrepareToUpdate( (TextWidget) w);
}

void 
#if NeedFunctionPrototypes
XawTextEnableRedisplay(Widget w)
#else
XawTextEnableRedisplay(w)
Widget w;
#endif
{
  TextWidget ctx = (TextWidget)w;
  XawTextPosition lastPos;

  if (!ctx->text.update_disabled) return;

  ctx->text.update_disabled = False;
  lastPos = ctx->text.lastPos = GETLASTPOS;
  ctx->text.lt.top = FindGoodPosition(ctx, ctx->text.lt.top);
  ctx->text.insertPos = FindGoodPosition(ctx, ctx->text.insertPos);
  if ( (ctx->text.s.left > lastPos) || (ctx->text.s.right > lastPos) ) 
    ctx->text.s.left = ctx->text.s.right = 0;
  
  _XawTextBuildLineTable(ctx, ctx->text.lt.top, TRUE);
  if (XtIsRealized(w))
    DisplayTextWindow(w);
  _XawTextExecuteUpdate(ctx);
}

Widget
#if NeedFunctionPrototypes
XawTextGetSource(Widget w)
#else
XawTextGetSource(w)
Widget w;
#endif
{
  return ((TextWidget)w)->text.source;
}

void
#if NeedFunctionPrototypes
XawTextDisplayCaret (Widget w,
#if NeedWidePrototypes
		    /* Boolean */ int display_caret)
#else
		    Boolean display_caret)
#endif
#else
XawTextDisplayCaret (w, display_caret)
Widget w;
Boolean display_caret;
#endif
{
  TextWidget ctx = (TextWidget) w;

  if (ctx->text.display_caret == display_caret) return;

  if (XtIsRealized(w)) {
    _XawTextPrepareToUpdate(ctx);
    ctx->text.display_caret = display_caret;
    _XawTextExecuteUpdate(ctx);
  }
  else
    ctx->text.display_caret = display_caret;
}

/*	Function Name: XawTextSearch(w, dir, text).
 *	Description: searches for the given text block.
 *	Arguments: w - The text widget.
 *                 dir - The direction to search. 
 *                 text - The text block containing info about the string
 *                        to search for.
 *	Returns: The position of the text found, or XawTextSearchError on 
 *               an error.
 */

XawTextPosition
#if NeedFunctionPrototypes
XawTextSearch(Widget w,
#if NeedWidePrototypes
	    /* XawTextScanDirection */ int dir,
#else
	    XawTextScanDirection dir,
#endif
	    XawTextBlock *text)
#else
XawTextSearch(w, dir, text) 
Widget w;
XawTextScanDirection dir;
XawTextBlock * text;
#endif
{
  TextWidget ctx = (TextWidget) w;

  return(SrcSearch(ctx->text.source, ctx->text.insertPos, dir, text));
}
  
TextClassRec textClassRec = {
  { /* core fields */
    /* superclass       */      (WidgetClass) &simpleClassRec,
    /* class_name       */      "Text",
    /* widget_size      */      sizeof(TextRec),
    /* class_initialize */      ClassInitialize,
    /* class_part_init  */	NULL,
    /* class_inited     */      FALSE,
    /* initialize       */      Initialize,
    /* initialize_hook  */	NULL,
    /* realize          */      Realize,
    /* actions          */      _XawTextActionsTable,
    /* num_actions      */      0,                /* Set in ClassInitialize. */
    /* resources        */      resources,
    /* num_ resource    */      XtNumber(resources),
    /* xrm_class        */      NULLQUARK,
    /* compress_motion  */      TRUE,
    /* compress_exposure*/      XtExposeGraphicsExpose | XtExposeNoExpose,
    /* compress_enterleave*/	TRUE,
    /* visible_interest */      FALSE,
    /* destroy          */      TextDestroy,
    /* resize           */      Resize,
    /* expose           */      ProcessExposeRegion,
    /* set_values       */      SetValues,
    /* set_values_hook  */	NULL,
    /* set_values_almost*/	XtInheritSetValuesAlmost,
    /* get_values_hook  */	GetValuesHook,
    /* accept_focus     */      NULL,
    /* version          */	XtVersion,
    /* callback_private */      NULL,
    /* tm_table         */      NULL,    /* set in ClassInitialize */
    /* query_geometry   */	XtInheritQueryGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension	*/	NULL
  },
  { /* Simple fields */
    /* change_sensitive	*/	ChangeSensitive
  },
  { /* text fields */
    /* empty            */	0
  }
};

WidgetClass textWidgetClass = (WidgetClass)&textClassRec;
