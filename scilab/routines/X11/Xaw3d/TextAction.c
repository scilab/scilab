/* $XConsortium: TextAction.c,v 1.53 95/06/14 15:07:27 kaleb Exp $ */

/*

Copyright (c) 1989, 1994  X Consortium

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

*/
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Misc.h>
#include <X11/Xmu/StdSel.h>		/* for XmuConvertStandardSelection */
#include <X11/Xmu/Atoms.h>		/* for XA_COMPOUND_TEXT */
#include <X11/Xaw3d/TextP.h>
#include <X11/Xaw3d/MultiSrcP.h>
#include <X11/Xaw3d/XawImP.h>
#include <X11/Xfuncs.h>
#include "XawI18n.h"
#include <stdio.h>
#include <ctype.h>

#define SrcScan                XawTextSourceScan
#define FindDist               XawTextSinkFindDistance
#define FindPos                XawTextSinkFindPosition

#define XawTextActionMaxHexChars 100

/*
 * These are defined in TextPop.c
 */

void _XawTextInsertFileAction(), _XawTextInsertFile(), _XawTextSearch();
void _XawTextSearch(), _XawTextDoSearchAction(), _XawTextDoReplaceAction();
void _XawTextSetField(), _XawTextPopdownSearchAction();

/*
 * These are defined in Text.c
 */

char * _XawTextGetText();
void _XawTextAlterSelection(), _XawTextVScroll();
void _XawTextSetSelection(), _XawTextCheckResize(), _XawTextExecuteUpdate();
void _XawTextSetScrollBars(), _XawTextClearAndCenterDisplay();
Atom * _XawTextSelectionList();
void _XawTextPrepareToUpdate();
int _XawTextReplace();

static void ParameterError(w, param)
    Widget w; 
    String param;
{
    String params[2];
    Cardinal num_params = 2;
    params[0] = XtName(w);
    params[1] = param;
    
    XtAppWarningMsg( XtWidgetToApplicationContext(w),
	"parameterError", "textAction", "XawError",
	"Widget: %s Parameter: %s", 
	params, &num_params);
    XBell( XtDisplay( w ), 50 );
}

static void
StartAction(ctx, event)
TextWidget ctx;
XEvent *event;
{
  _XawTextPrepareToUpdate(ctx);
  if (event != NULL) {
    switch (event->type) {
    case ButtonPress:
    case ButtonRelease:
      ctx->text.time = event->xbutton.time;
      break;
    case KeyPress:
    case KeyRelease:
      ctx->text.time = event->xkey.time;
      break;
    case MotionNotify:
      ctx->text.time = event->xmotion.time;
      break;
    case EnterNotify:
    case LeaveNotify:
      ctx->text.time = event->xcrossing.time;
    }
  }
}

static void
NotePosition(ctx, event)
TextWidget ctx;
XEvent* event;
{
  switch (event->type) {
  case ButtonPress:
  case ButtonRelease:
    ctx->text.ev_x = event->xbutton.x;
    ctx->text.ev_y = event->xbutton.y;
    break;
  case KeyPress:
  case KeyRelease:
    {
      XRectangle cursor;
      XawTextSinkGetCursorBounds(ctx->text.sink, &cursor);
      ctx->text.ev_x = cursor.x + cursor.width / 2;;
      ctx->text.ev_y = cursor.y + cursor.height / 2;;
    }
    break;
  case MotionNotify:
    ctx->text.ev_x = event->xmotion.x;
    ctx->text.ev_y = event->xmotion.y;
    break;
  case EnterNotify:
  case LeaveNotify:
    ctx->text.ev_x = event->xcrossing.x;
    ctx->text.ev_y = event->xcrossing.y;
  }
}

static void
EndAction(ctx)
TextWidget ctx;
{
  _XawTextCheckResize(ctx);
  _XawTextExecuteUpdate(ctx);
  ctx->text.mult = 1;
}


struct _SelectionList {
    String* params;
    Cardinal count;
    Time time;
    Boolean CT_asked;	/* flag if asked XA_COMPOUND_TEXT */
    Atom selection;	/* selection atom when asking XA_COMPOUND_TEXT */
};

static int ProbablyMB(s)
    char* s;
{
    int escapes = 0;
    int has_hi_bit = False;

    /* if it has more than one ESC char, I assume it is COMPOUND_TEXT.
    If it has at least one hi bit set character, I pretend it is multibyte. */

    while ( (wchar_t)(*s) != (wchar_t)0 ) {
        if ( *s & 128 )
            has_hi_bit = True;
        if ( *s++ == '\033' )
            escapes++;
        if ( escapes >= 2 )
            return( 0 );
    }
    return( has_hi_bit );
}
static void GetSelection();

/* ARGSUSED */
static void 
_SelectionReceived(w, client_data, selection, type, value, length, format)
Widget w;
XtPointer client_data;
Atom *selection, *type;
XtPointer value;
unsigned long *length;
int* format;
{
  TextWidget ctx = (TextWidget)w;
  XawTextBlock text;
  
  if (*type == 0 /*XT_CONVERT_FAIL*/ || *length == 0) {
    struct _SelectionList* list = (struct _SelectionList*)client_data;
    if (list != NULL) {
      if (list->CT_asked) {

	/* If we just asked for a XA_COMPOUND_TEXT and got a null
	response, we'll ask again, this time for an XA_STRING. */

	list->CT_asked = False;
        XtGetSelectionValue(w, list->selection, XA_STRING, _SelectionReceived,
                            (XtPointer)list, list->time);
      } else {
	GetSelection(w, list->time, list->params, list->count);
	XtFree(client_data);
     }
    }
    return;
  }

  /* Many programs, especially old terminal emulators, give us multibyte text
but tell us it is COMPOUND_TEXT :(  The following routine checks to see if the
string is a legal multibyte string in our locale using a spooky heuristic :O
and if it is we can only assume the sending client is using the same locale as
we are, and convert it.  I also warn the user that the other client is evil. */

  StartAction( ctx, (XEvent*) NULL );
  if (_XawTextFormat(ctx) == XawFmtWide) {
      XTextProperty textprop;
      Display *d = XtDisplay((Widget)ctx);
      wchar_t **wlist;
      int count;
      int try_CT = 1;

      /* IS THE SELECTION IN MULTIBYTE FORMAT? */

      if ( ProbablyMB( (char *) value ) ) {
          char * list[1];
          list[0] = (char *) value;
          if ( XmbTextListToTextProperty( d, (char**) list, 1,
				XCompoundTextStyle, &textprop ) == Success )
              try_CT = 0;
      }

      /* OR IN COMPOUND TEXT FORMAT? */

      if ( try_CT ) {
          textprop.encoding = XA_COMPOUND_TEXT(d);
          textprop.value = (unsigned char *)value;
          textprop.nitems = strlen(value);
          textprop.format = 8;
      }

      if ( XwcTextPropertyToTextList( d, &textprop, (wchar_t***) &wlist, &count )
		!=  Success) {
          XwcFreeStringList( (wchar_t**) wlist );

          /* Notify the user on strerr and in the insertion :) */
          textprop.value = (unsigned char *) " >> ILLEGAL SELECTION << ";
          count = 1;
          fprintf( stderr, "Xaw Text Widget: An attempt was made to insert an illegal selection.\n" );

          if ( XwcTextPropertyToTextList( d, &textprop, (wchar_t***) &wlist, &count )
		!=  Success) return;
      }

      XFree(value);
      value = (XPointer)wlist[0];

      *length = wcslen(wlist[0]);
      XtFree((XtPointer)wlist);
      text.format = XawFmtWide;
  } else
      text.format = XawFmt8Bit;
  text.ptr = (char*)value;
  text.firstPos = 0;
  text.length = *length;
  if (_XawTextReplace(ctx, ctx->text.insertPos, ctx->text.insertPos, &text)) {
    XBell(XtDisplay(ctx), 0);
    return;
  }
  ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos, 
				XawstPositions, XawsdRight, text.length, TRUE);

  _XawTextSetScrollBars(ctx);
  EndAction(ctx);
  XtFree(client_data);
  XFree(value);		/* the selection value should be freed with XFree */
}

static void 
GetSelection(w, time, params, num_params)
Widget w;
Time time;
String* params;		/* selections in precedence order */
Cardinal num_params;
{
    Atom selection;
    int buffer;

    selection = XInternAtom(XtDisplay(w), *params, False);

    switch (selection) {
      case XA_CUT_BUFFER0: buffer = 0; break;
      case XA_CUT_BUFFER1: buffer = 1; break;
      case XA_CUT_BUFFER2: buffer = 2; break;
      case XA_CUT_BUFFER3: buffer = 3; break;
      case XA_CUT_BUFFER4: buffer = 4; break;
      case XA_CUT_BUFFER5: buffer = 5; break;
      case XA_CUT_BUFFER6: buffer = 6; break;
      case XA_CUT_BUFFER7: buffer = 7; break;
      default:	       buffer = -1;
    }
    if (buffer >= 0) {
	int nbytes;
	unsigned long length;
	int fmt8 = 8;
	Atom type = XA_STRING;
	char *line = XFetchBuffer(XtDisplay(w), &nbytes, buffer);
	if ((length = nbytes))
	    _SelectionReceived(w, (XtPointer) NULL, &selection, &type, (XPointer)line,
			       &length, &fmt8);
	else if (num_params > 1)
	    GetSelection(w, time, params+1, num_params-1);
    } else {
	struct _SelectionList* list;
	if (--num_params) {
	    list = XtNew(struct _SelectionList);
	    list->params = params + 1;
	    list->count = num_params;
	    list->time = time;
	    list->CT_asked = True;
	    list->selection = selection;
	} else list = NULL;
	XtGetSelectionValue(w, selection, XA_COMPOUND_TEXT(XtDisplay(w)),
			    _SelectionReceived, (XtPointer)list, time);
    }
}

static void 
InsertSelection(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;		/* precedence list of selections to try */
Cardinal* num_params;
{
  StartAction((TextWidget)w, event); /* Get Time. */
  GetSelection(w, ((TextWidget)w)->text.time, params, *num_params);
  EndAction((TextWidget)w);
}

/************************************************************
 *
 * Routines for Moving Around.
 *
 ************************************************************/

static void
Move(ctx, event, dir, type, include)
TextWidget ctx;
XEvent* event;
XawTextScanDirection dir;
XawTextScanType type;
Boolean include;
{
  StartAction(ctx, event);
  ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos,
				type, dir, ctx->text.mult, include);
  EndAction(ctx);
}

/*ARGSUSED*/
static void 
MoveForwardChar(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
   Move((TextWidget) w, event, XawsdRight, XawstPositions, TRUE);
}

/*ARGSUSED*/
static void 
MoveBackwardChar(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstPositions, TRUE);
}

/*ARGSUSED*/
static void 
MoveForwardWord(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdRight, XawstWhiteSpace, FALSE);
}

/*ARGSUSED*/
static void 
MoveBackwardWord(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstWhiteSpace, FALSE);
}

/*ARGSUSED*/
static void MoveForwardParagraph(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdRight, XawstParagraph, FALSE);
}

/*ARGSUSED*/
static void MoveBackwardParagraph(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstParagraph, FALSE);
}

/*ARGSUSED*/
static void 
MoveToLineEnd(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdRight, XawstEOL, FALSE);
}

/*ARGSUSED*/
static void 
MoveToLineStart(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstEOL, FALSE);
}


static void
MoveLine(ctx, event, dir)
TextWidget ctx;
XEvent* event;
XawTextScanDirection dir;
{
  XawTextPosition new, next_line, junk;
  int from_left, garbage;

  StartAction(ctx, event);

  if (dir == XawsdLeft)
    ctx->text.mult++;

  new = SrcScan(ctx->text.source, ctx->text.insertPos,
		XawstEOL, XawsdLeft, 1, FALSE);

  FindDist(ctx->text.sink, new, ctx->text.margin.left, ctx->text.insertPos,
	   &from_left, &junk, &garbage);

  new = SrcScan(ctx->text.source, ctx->text.insertPos, XawstEOL, dir,
		ctx->text.mult, (dir == XawsdRight));

  next_line = SrcScan(ctx->text.source, new, XawstEOL, XawsdRight, 1, FALSE);

  FindPos(ctx->text.sink, new, ctx->text.margin.left, from_left, FALSE,
	  &(ctx->text.insertPos), &garbage, &garbage);
  
  if (ctx->text.insertPos > next_line)
    ctx->text.insertPos = next_line;

  EndAction(ctx);
}

/*ARGSUSED*/
static void 
MoveNextLine(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  MoveLine( (TextWidget) w, event, XawsdRight);
}

/*ARGSUSED*/
static void 
MovePreviousLine(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  MoveLine( (TextWidget) w, event, XawsdLeft);
}

/*ARGSUSED*/
static void
MoveBeginningOfFile(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstAll, TRUE);
}

/*ARGSUSED*/
static void 
MoveEndOfFile(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Move((TextWidget) w, event, XawsdRight, XawstAll, TRUE);
}

static void 
Scroll(ctx, event, dir)
TextWidget ctx;
XEvent* event;
XawTextScanDirection dir;
{
  StartAction(ctx, event);

  if (dir == XawsdLeft)
    _XawTextVScroll(ctx, ctx->text.mult);
  else
    _XawTextVScroll(ctx, -ctx->text.mult);

  EndAction(ctx);
}

/*ARGSUSED*/
static void 
ScrollOneLineUp(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Scroll( (TextWidget) w, event, XawsdLeft);
}

/*ARGSUSED*/
static void 
ScrollOneLineDown(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  Scroll( (TextWidget) w, event, XawsdRight);
}

static void 
MovePage(ctx, event, dir)
TextWidget ctx;
XEvent* event;
XawTextScanDirection dir;
{
  int scroll_val = Max(1, ctx->text.lt.lines - 2);

  if (dir == XawsdLeft)
    scroll_val = -scroll_val;

  StartAction(ctx, event);
  _XawTextVScroll(ctx, scroll_val);
  ctx->text.insertPos = ctx->text.lt.top;
  EndAction(ctx);
}

/*ARGSUSED*/
static void 
MoveNextPage(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  MovePage((TextWidget) w, event, XawsdRight);
}

/*ARGSUSED*/
static void 
MovePreviousPage(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  MovePage((TextWidget) w, event, XawsdLeft);
}

/************************************************************
 *
 * Delete Routines.
 *
 ************************************************************/

static Boolean
MatchSelection(selection, s)
    Atom selection;
    XawTextSelection* s;
{
    Atom    *match;
    int	    count;

    for (count = 0, match = s->selections; count < s->atom_count; match++, count++)
	if (*match == selection)
	    return True;
    return False;
}

#define SrcCvtSel	XawTextSourceConvertSelection

static Boolean 
ConvertSelection(w, selection, target, type, value, length, format)
    Widget w;
    Atom *selection, *target, *type;
    XtPointer* value;
    unsigned long* length;
    int* format;
{
  Display* d = XtDisplay(w);
  TextWidget ctx = (TextWidget)w;
  Widget src = ctx->text.source;
  XawTextEditType edit_mode;
  Arg args[1];
  XawTextSelectionSalt	*salt = NULL;
  XawTextSelection  *s;

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
    XtGetValues(src, args, 1);

    if (edit_mode == XawtextEdit) {
      *targetP++ = XA_DELETE(d);
      (*length)++;
    }
    memcpy((char*)targetP, (char*)std_targets, sizeof(Atom)*std_length);
    XtFree((char*)std_targets);
    *type = XA_ATOM;
    *format = 32;
    return True;
  }
  
  if ( SrcCvtSel(src, selection, target, type, value, length, format) )
    return True;

  for (salt = ctx->text.salt2; salt; salt = salt->next)
    if (MatchSelection (*selection, &salt->s))
      break;
  if (!salt)
    return False;
  s = &salt->s;
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
	    *value = (char *)_XawTextGetSTRING(ctx, s->left, s->right);
	    if (_XawTextFormat(ctx) == XawFmtWide) {
		XTextProperty textprop;
		if (XwcTextListToTextProperty(d, (wchar_t**)value, 1,
					      XCompoundTextStyle, &textprop)
			< Success) {
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
	    wchar_t** wlist;
	    int count;
	    textprop.encoding = XA_COMPOUND_TEXT(d);
	    textprop.value = (unsigned char *)*value;
	    textprop.nitems = strlen(*value);
	    textprop.format = 8;
	    if (XwcTextPropertyToTextList(d, &textprop, (wchar_t***)&wlist, &count)
			< Success) {
		XtFree(*value);
		return False;
	    }
	    XtFree(*value);
	    if (XwcTextListToTextProperty(d, (wchar_t**)wlist, 1,
					  XStringStyle, &textprop) < Success) {
		XwcFreeStringList( (wchar_t**) wlist );
		return False;
	    }
	    *value = (XtPointer)textprop.value;
	    *length = textprop.nitems;
	    XwcFreeStringList( (wchar_t**) wlist );
	}
	*format = 8;
	return True;
  }

  if ( (*target == XA_LIST_LENGTH(d)) || (*target == XA_LENGTH(d)) ) {
    long * temp;
    
    temp = (long *) XtMalloc(sizeof(long));
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
    
    temp = (long *) XtMalloc(2 * sizeof(long));
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

static void 
LoseSelection(w, selection)
    Widget w;
    Atom* selection;
{
  TextWidget ctx = (TextWidget) w;
  Atom* atomP;
  int i;
  XawTextSelectionSalt	*salt, *prevSalt, *nextSalt;

    prevSalt = 0;
    for (salt = ctx->text.salt2; salt; salt = nextSalt)
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

            /* WARNING: the next line frees memory not allocated in Xaw. */
            /* Could be a serious bug.  Someone look into it. */
	    XtFree (salt->contents);
	    if (prevSalt)
		prevSalt->next = nextSalt;
	    else
		ctx->text.salt2 = nextSalt;
	    XtFree ((char *) salt);
	}
	else
	    prevSalt = salt;
    }
}

static void 
_DeleteOrKill(ctx, from, to, kill)
TextWidget ctx;
XawTextPosition from, to;
Boolean	kill;
{
  XawTextBlock text;
  
  if (kill && from < to) {
    XawTextSelectionSalt    *salt;
    Atom selection = XInternAtom(XtDisplay(ctx), "SECONDARY", False);

    LoseSelection ((Widget) ctx, &selection);
    salt = (XawTextSelectionSalt *) XtMalloc (sizeof (XawTextSelectionSalt));
    if (!salt)
	return;
    salt->s.selections = (Atom *) XtMalloc (sizeof (Atom));
    if (!salt->s.selections)
    {
	XtFree ((char *) salt);
	return;
    }
    salt->s.left = from;
    salt->s.right = to;
    salt->contents = (char *)_XawTextGetSTRING(ctx, from, to);
    if (_XawTextFormat(ctx) == XawFmtWide) {
	XTextProperty textprop;
	if (XwcTextListToTextProperty(XtDisplay((Widget)ctx),
			(wchar_t**)(&(salt->contents)), 1, XCompoundTextStyle,
			&textprop) <  Success) {
	    XtFree(salt->contents);
	    salt->length = 0;
	    return;
	}
	XtFree(salt->contents);
	salt->contents = (char *)textprop.value;
	salt->length = textprop.nitems;
    } else
       salt->length = strlen (salt->contents);
    salt->next = ctx->text.salt2;
    ctx->text.salt2 = salt;
    salt->s.selections[0] = selection;
    XtOwnSelection ((Widget) ctx, selection, ctx->text.time,
		    ConvertSelection, LoseSelection, NULL);
    salt->s.atom_count = 1;
/*
    XStoreBuffer(XtDisplay(ctx), ptr, strlen(ptr), 1);
    XtFree(ptr);
*/
  }
  text.length = 0;
  text.firstPos = 0;

  text.format = _XawTextFormat(ctx);
  text.ptr = "";	/* These two lines needed to make legal TextBlock */

  if (_XawTextReplace(ctx, from, to, &text)) {
    XBell(XtDisplay(ctx), 50);
    return;
  }
  ctx->text.insertPos = from;
  ctx->text.showposition = TRUE; 
}

static void
DeleteOrKill(ctx, event, dir, type, include, kill)
TextWidget	   ctx;
XEvent* event;
XawTextScanDirection dir;
XawTextScanType type;
Boolean	   include, kill;
{
  XawTextPosition from, to;
  
  StartAction(ctx, event);
  to = SrcScan(ctx->text.source, ctx->text.insertPos,
	       type, dir, ctx->text.mult, include);

/*
 * If no movement actually happened, then bump the count and try again. 
 * This causes the character position at the very beginning and end of 
 * a boundary to act correctly. 
 */

  if (to == ctx->text.insertPos)
      to = SrcScan(ctx->text.source, ctx->text.insertPos,
		   type, dir, ctx->text.mult + 1, include);

  if (dir == XawsdLeft) {
    from = to;
    to = ctx->text.insertPos;
  }
  else 
    from = ctx->text.insertPos;

  _DeleteOrKill(ctx, from, to, kill);
  _XawTextSetScrollBars(ctx);
  EndAction(ctx);
}

/*ARGSUSED*/
static void 
DeleteForwardChar(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  DeleteOrKill((TextWidget) w, event, XawsdRight, XawstPositions, TRUE, FALSE);
}

/*ARGSUSED*/
static void
DeleteBackwardChar(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  DeleteOrKill((TextWidget) w, event, XawsdLeft, XawstPositions, TRUE, FALSE);
}

/*ARGSUSED*/
static void 
DeleteForwardWord(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  DeleteOrKill((TextWidget) w, event,
	       XawsdRight, XawstWhiteSpace, FALSE, FALSE);
}

/*ARGSUSED*/
static void 
DeleteBackwardWord(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  DeleteOrKill((TextWidget) w, event,
	       XawsdLeft, XawstWhiteSpace, FALSE, FALSE);
}

/*ARGSUSED*/
static void 
KillForwardWord(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  DeleteOrKill((TextWidget) w, event, 
	       XawsdRight, XawstWhiteSpace, FALSE, TRUE);
}

/*ARGSUSED*/
static void 
KillBackwardWord(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  DeleteOrKill((TextWidget) w, event,
	       XawsdLeft, XawstWhiteSpace, FALSE, TRUE);
}

/*ARGSUSED*/
static void
KillToEndOfLine(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  TextWidget ctx = (TextWidget) w;
  XawTextPosition end_of_line;

  StartAction(ctx, event);
  end_of_line = SrcScan(ctx->text.source, ctx->text.insertPos, XawstEOL, 
			XawsdRight, ctx->text.mult, FALSE);
  if (end_of_line == ctx->text.insertPos)
    end_of_line = SrcScan(ctx->text.source, ctx->text.insertPos, XawstEOL, 
			  XawsdRight, ctx->text.mult, TRUE);

  _DeleteOrKill(ctx, ctx->text.insertPos, end_of_line, TRUE);
  _XawTextSetScrollBars(ctx);
  EndAction(ctx);
}

/*ARGSUSED*/
static void 
KillToEndOfParagraph(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  DeleteOrKill((TextWidget) w, event, XawsdRight, XawstParagraph, FALSE, TRUE);
}

void 
_XawTextZapSelection(ctx, event, kill)
TextWidget ctx;
XEvent* event;
Boolean kill;
{
   StartAction(ctx, event);
   _DeleteOrKill(ctx, ctx->text.s.left, ctx->text.s.right, kill);
  _XawTextSetScrollBars(ctx);
   EndAction(ctx);
}

/*ARGSUSED*/
static void 
KillCurrentSelection(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  _XawTextZapSelection( (TextWidget) w, event, TRUE);
}

/*ARGSUSED*/
static void 
DeleteCurrentSelection(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  _XawTextZapSelection( (TextWidget) w, event, FALSE);
}

/************************************************************
 *
 * Insertion Routines.
 *
 ************************************************************/

static int 
InsertNewLineAndBackupInternal(ctx)
TextWidget ctx;
{
  int count, error = XawEditDone;
  XawTextBlock text;

  text.format = _XawTextFormat(ctx);
  text.length = ctx->text.mult;
  text.firstPos = 0;

  if ( text.format == XawFmtWide ) {
      wchar_t* wptr;
      text.ptr =  XtMalloc(sizeof(wchar_t) * ctx->text.mult);
      wptr = (wchar_t *)text.ptr;
      for (count = 0; count < ctx->text.mult; count++ )
          wptr[count] = _Xaw_atowc(XawLF);
  }
  else {
      text.ptr = XtMalloc(sizeof(char) * ctx->text.mult);
      for (count = 0; count < ctx->text.mult; count++ )
          text.ptr[count] = XawLF;
  }

  if (_XawTextReplace(ctx, ctx->text.insertPos, ctx->text.insertPos, &text)) {
    XBell( XtDisplay(ctx), 50);
    error = XawEditError;
  }
  else 
    ctx->text.showposition = TRUE;

  XtFree( text.ptr );
  return( error );
}

/*ARGSUSED*/
static void 
InsertNewLineAndBackup(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  StartAction( (TextWidget) w, event );
  (void) InsertNewLineAndBackupInternal( (TextWidget) w );
  _XawTextSetScrollBars( (TextWidget) w);
  EndAction( (TextWidget) w );
}

static int
LocalInsertNewLine(ctx, event)
    TextWidget ctx;
    XEvent* event;
{
  StartAction(ctx, event);
  if (InsertNewLineAndBackupInternal(ctx) == XawEditError)
    return(XawEditError);
  ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos, 
			     XawstPositions, XawsdRight, ctx->text.mult, TRUE);
  _XawTextSetScrollBars(ctx);
  EndAction(ctx);
  return(XawEditDone);
}

/*ARGSUSED*/
static void
InsertNewLine(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  (void) LocalInsertNewLine( (TextWidget) w, event);
}

/*ARGSUSED*/
static void 
InsertNewLineAndIndent(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  XawTextBlock text;
  XawTextPosition pos1;
  int length;
  TextWidget ctx = (TextWidget) w;
  String line_to_ip;

  StartAction(ctx, event);
  pos1 = SrcScan(ctx->text.source, ctx->text.insertPos, 
		 XawstEOL, XawsdLeft, 1, FALSE);

  line_to_ip = _XawTextGetText(ctx, pos1, ctx->text.insertPos);

  text.format = _XawTextFormat(ctx);
  text.firstPos = 0;

  if ( text.format == XawFmtWide ) {
     wchar_t* ptr;
     text.ptr = XtMalloc( ( 2 + wcslen((wchar_t*)line_to_ip) ) * sizeof(wchar_t) );

     ptr = (wchar_t*)text.ptr;
     ptr[0] = _Xaw_atowc( XawLF );
     wcscpy( (wchar_t*) ++ptr, (wchar_t*) line_to_ip );

     length = wcslen((wchar_t*)text.ptr);
     while ( length && ( iswspace(*ptr) || ( *ptr == _Xaw_atowc(XawTAB) ) ) )
         ptr++, length--;
     *ptr = (wchar_t)0;
     text.length = wcslen((wchar_t*)text.ptr);

  } else {
     char *ptr;
     length = strlen(line_to_ip);
     /* The current line + \0 and LF will be copied to this 
	buffer. Before my fix, only length + 1 bytes were
	allocated, causing on machine with non-wasteful 
	malloc implementation segmentation violations by
	overwriting the bypte after the allocated area

	-gustaf neumann
      */
     text.ptr = XtMalloc( ( 2 + length ) * sizeof( char ) );

     ptr = text.ptr;
     ptr[0] = XawLF;
     strcpy( ++ptr, line_to_ip );

     length++;
     while ( length && ( isspace(*ptr) || ( *ptr == XawTAB ) ) )
         ptr++, length--;
     *ptr = '\0';
     text.length = strlen(text.ptr);
  }
  XtFree( line_to_ip );

  if (_XawTextReplace(ctx,ctx->text.insertPos, ctx->text.insertPos, &text)) {
    XBell(XtDisplay(ctx), 50);
    XtFree(text.ptr);
    EndAction(ctx);
    return;
  }
  XtFree(text.ptr);
  ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos,
				XawstPositions, XawsdRight, text.length, TRUE);
  _XawTextSetScrollBars(ctx);
  EndAction(ctx);
}

/************************************************************
 *
 * Selection Routines.
 *
 *************************************************************/

static void 
SelectWord(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
  TextWidget ctx = (TextWidget) w;
  XawTextPosition l, r;

  StartAction(ctx, event);
  l = SrcScan(ctx->text.source, ctx->text.insertPos, 
	      XawstWhiteSpace, XawsdLeft, 1, FALSE);
  r = SrcScan(ctx->text.source, l, XawstWhiteSpace, XawsdRight, 1, FALSE);
  _XawTextSetSelection(ctx, l, r, params, *num_params);
  EndAction(ctx);
}

static void 
SelectAll(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
  TextWidget ctx = (TextWidget) w;

  StartAction(ctx, event);
  _XawTextSetSelection(ctx,zeroPosition,ctx->text.lastPos,params,*num_params);
  EndAction(ctx);
}

static void
ModifySelection(ctx, event, mode, action, params, num_params)
TextWidget ctx;
XEvent* event;
XawTextSelectionMode mode;
XawTextSelectionAction action;
String* params;		/* unused */
Cardinal* num_params;	/* unused */
{
  StartAction(ctx, event);
  NotePosition(ctx, event);
  _XawTextAlterSelection(ctx, mode, action, params, num_params);
  EndAction(ctx);
}
		
/* ARGSUSED */
static void 
SelectStart(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;		/* unused */
Cardinal* num_params;	/* unused */
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextSelect, XawactionStart, params, num_params);
}

/* ARGSUSED */
static void 
SelectAdjust(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;		/* unused */
Cardinal* num_params;	/* unused */
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextSelect, XawactionAdjust, params, num_params);
}

static void 
SelectEnd(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextSelect, XawactionEnd, params, num_params);
}

/* ARGSUSED */
static void 
ExtendStart(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;		/* unused */
Cardinal* num_params;	/* unused */
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextExtend, XawactionStart, params, num_params);
}

/* ARGSUSED */
static void 
ExtendAdjust(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;		/* unused */
Cardinal* num_params;	/* unused */
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextExtend, XawactionAdjust, params, num_params);
}

static void 
ExtendEnd(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextExtend, XawactionEnd, params, num_params);
}

static void
SelectSave(w, event, params, num_params)
Widget  w;
XEvent* event;
String* params;
Cardinal* num_params;
{
    int	    num_atoms;
    Atom*   sel;
    Display* dpy = XtDisplay(w);
    Atom    selections[256];

    StartAction(  (TextWidget) w, event );
    num_atoms = *num_params;
    if (num_atoms > 256)
	num_atoms = 256;
    for (sel=selections; --num_atoms >= 0; sel++, params++)
	*sel = XInternAtom(dpy, *params, False);
    num_atoms = *num_params;
    _XawTextSaltAwaySelection( (TextWidget) w, selections, num_atoms );
    EndAction(  (TextWidget) w );
}

/************************************************************
 *
 * Misc. Routines.
 *
 ************************************************************/

/* ARGSUSED */
static void 
RedrawDisplay(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  StartAction( (TextWidget) w, event);
  _XawTextClearAndCenterDisplay((TextWidget) w);
  EndAction( (TextWidget) w);
}

/*ARGSUSED*/
static void
TextFocusIn (w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  TextWidget ctx = (TextWidget) w;

  /* Let the input method know focus has arrived. */
  _XawImSetFocusValues (w, NULL, 0);
  if ( event->xfocus.detail == NotifyPointer ) return;

  ctx->text.hasfocus = TRUE; 
}

/*ARGSUSED*/
static void
TextFocusOut(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  TextWidget ctx = (TextWidget) w;

  /* Let the input method know focus has left.*/
  _XawImUnsetFocus(w);
  if ( event->xfocus.detail == NotifyPointer ) return;
  ctx->text.hasfocus = FALSE;
}

/*ARGSUSED*/
static void
TextEnterWindow( w, event, params, num_params )
    Widget w;
    XEvent* event;
    String* params;
    Cardinal* num_params;
{
  TextWidget ctx = (TextWidget) w;

  if ((event->xcrossing.detail != NotifyInferior) && event->xcrossing.focus &&
      !ctx->text.hasfocus) {
	_XawImSetFocusValues(w, NULL, 0);
  }
}

/*ARGSUSED*/
static void
TextLeaveWindow( w, event, params, num_params )
    Widget w;
    XEvent* event;
    String* params;
    Cardinal* num_params;
{
  TextWidget ctx = (TextWidget) w;

  if ((event->xcrossing.detail != NotifyInferior) && event->xcrossing.focus &&
      !ctx->text.hasfocus) {
	_XawImUnsetFocus(w);
  }
}

static XComposeStatus compose_status = {NULL, 0};

/*	Function Name: AutoFill
 *	Description: Breaks the line at the previous word boundry when
 *                   called inside InsertChar.
 *	Arguments: ctx - The text widget.
 *	Returns: none
 */

static void
AutoFill(ctx)
TextWidget ctx;
{
  int width, height, x, line_num, max_width;
  XawTextPosition ret_pos;
  XawTextBlock text;

  if ( !((ctx->text.auto_fill) && (ctx->text.mult == 1)) )
    return;

  for ( line_num = 0; line_num < ctx->text.lt.lines ; line_num++)
    if ( ctx->text.lt.info[line_num].position >= ctx->text.insertPos )
      break;
  line_num--;			/* backup a line. */

  max_width = Max(0, (int)(ctx->core.width - HMargins(ctx)));

  x = ctx->text.margin.left;
  XawTextSinkFindPosition( ctx->text.sink,ctx->text.lt.info[line_num].position,
			  x, max_width, TRUE, &ret_pos, &width, &height);
  
  if ( ret_pos >= ctx->text.insertPos )
    return;
  
  text.format = XawFmt8Bit;
  if (_XawTextFormat(ctx) == XawFmtWide) {
    text.format = XawFmtWide;
    text.ptr =  (char *)XtMalloc(sizeof(wchar_t) * 2);
    ((wchar_t*)text.ptr)[0] = _Xaw_atowc(XawLF);
    ((wchar_t*)text.ptr)[1] = 0;
  } else
    text.ptr = "\n";
  text.length = 1;
  text.firstPos = 0;

  if (_XawTextReplace(ctx, ret_pos - 1, ret_pos, &text))
    XBell(XtDisplay((Widget) ctx), 0);	/* Unable to edit, complain. */
}

/*ARGSUSED*/
static void
InsertChar(w, event, p, n)
Widget w;
XEvent* event;
String* p;
Cardinal* n;
{
  TextWidget ctx = (TextWidget) w;
  char *ptr, strbuf[BUFSIZ];
  int count, error;
  KeySym keysym;
  XawTextBlock text;

  if (XtIsSubclass (ctx->text.source, (WidgetClass) multiSrcObjectClass))
    text.length = _XawImWcLookupString (w, &event->xkey,
		(wchar_t*) strbuf, BUFSIZ, &keysym, (Status*) &compose_status);
  else
    text.length = XLookupString ((XKeyEvent*)event, strbuf, BUFSIZ, &keysym, &compose_status);

  if (text.length == 0)
      return;

  text.format = _XawTextFormat( ctx );
  if ( text.format == XawFmtWide ) {
      text.ptr = ptr = XtMalloc(sizeof(wchar_t) * text.length * ctx->text.mult );
      for (count = 0; count < ctx->text.mult; count++ ) {
          memcpy((char*) ptr, (char *)strbuf, sizeof(wchar_t) * text.length );
          ptr += sizeof(wchar_t) * text.length;
      }

  } else { /* == XawFmt8Bit */
      text.ptr = ptr = XtMalloc( sizeof(char) * text.length * ctx->text.mult );
      for ( count = 0; count < ctx->text.mult; count++ ) {
          strncpy( ptr, strbuf, text.length );
          ptr += text.length;
      }
  }

  text.length = text.length * ctx->text.mult;
  text.firstPos = 0;

  StartAction(ctx, event);
  
  error = _XawTextReplace(ctx, ctx->text.insertPos,ctx->text.insertPos, &text);

  if (error == XawEditDone) {
      ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos,
	      XawstPositions, XawsdRight, text.length, TRUE);
      AutoFill(ctx);
  }
  else 
      XBell(XtDisplay(ctx), 50);

  XtFree(text.ptr);
  _XawTextSetScrollBars(ctx);
  EndAction(ctx);
}


/* IfHexConvertHexElseReturnParam() - called by InsertString
 *
 * i18n requires the ability to specify multiple characters in a hexa-
 * decimal string at once.  Since Insert was already too long, I made
 * this a seperate routine.
 *
 * A legal hex string in MBNF: '0' 'x' ( HEX-DIGIT HEX-DIGIT )+ '\0'
 *
 * WHEN:    the passed param is a legal hex string
 * RETURNS: a pointer to that converted, null terminated hex string;
 *          len_return holds the character count of conversion result
 *
 * WHEN:    the passed param is not a legal hex string:
 * RETURNS: the parameter passed;
 *          len_return holds the char count of param.
 *
 * NOTE:    In neither case will there be strings to free. */

static char* 
IfHexConvertHexElseReturnParam(param, len_return)
    char* param;
    int* len_return;
{
  char *p;                     /* steps through param char by char */
  char c;                      /* holds the character pointed to by p */

  int ind;		       /* steps through hexval buffer char by char */
  static char hexval[ XawTextActionMaxHexChars ];
  Boolean first_digit;

  /* reject if it doesn't begin with 0x and at least one more character. */

  if ( ( param[0] != '0' ) || ( param[1] != 'x' ) || ( param[2] == '\0' ) ) {
      *len_return = strlen( param );
      return( param );
  }

  /* Skip the 0x; go character by character shifting and adding. */

  first_digit = True;
  ind = 0;
  hexval[ ind ] = '\0';

  for ( p = param+2; ( c = *p ); p++ ) {
      hexval[ ind ] *= 16;
      if (c >= '0' && c <= '9')
          hexval[ ind ] += c - '0';
      else if (c >= 'a' && c <= 'f')
          hexval[ ind ] += c - 'a' + 10;
      else if (c >= 'A' && c <= 'F')
          hexval[ ind ] += c - 'A' + 10;
      else break;

      /* If we didn't break in preceding line, it was a good hex char. */

      if ( first_digit )
          first_digit = False;
      else {
          first_digit = True;
          if ( ++ind < XawTextActionMaxHexChars )
              hexval[ ind ] = '\0';
          else {
              *len_return = strlen( param );
              return( param );
          }
      }
  }

  /* We quit the above loop becasue we hit a non hex.  If that char is \0... */

  if ( ( c == '\0' ) && first_digit ) {
      *len_return = strlen( hexval );
      return( hexval );       /* ...it was a legal hex string, so return it.*/
  }

  /* Else, there were non-hex chars or odd digit count, so... */

  *len_return = strlen( param );
  return( param );			   /* ...return the verbatim string. */
}


/* InsertString() - action
 *
 * Mostly rewritten for R6 i18n.
 *
 * Each parameter, in turn, will be insert at the inputPos
 * and the inputPos advances to the insertion's end.
 *
 * The exception is that parameters composed of the two
 * characters 0x, followed only by an even number of
 * hexadecimal digits will be converted to characters. */

/*ARGSUSED*/
static void 
InsertString(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
  TextWidget ctx = (TextWidget) w;
  XtAppContext app_con = XtWidgetToApplicationContext(w);
  XawTextBlock text;
  int	   i;

  text.firstPos = 0;
  text.format = _XawTextFormat( ctx );

  StartAction(ctx, event);
  for ( i = *num_params; i; i--, params++ ) { /* DO FOR EACH PARAMETER */

      text.ptr = IfHexConvertHexElseReturnParam( *params, &text.length );

      if ( text.length == 0 ) continue;

      if ( _XawTextFormat( ctx ) == XawFmtWide ) { /* convert to WC */

          int temp_len;
          text.ptr = (char*) _XawTextMBToWC( XtDisplay(w), text.ptr,
					      &text.length );

          if ( text.ptr == NULL ) { /* conversion error */
              XtAppWarningMsg( app_con,
		"insertString", "textAction", "XawError",
		"insert-string()'s parameter contents not legal in this locale.",
		NULL, NULL );
              ParameterError( w, *params );
              continue;
          }

          /* Double check that the new input is legal: try to convert to MB. */

          temp_len = text.length;      /* _XawTextWCToMB's 3rd arg is in_out */ 
          if ( _XawTextWCToMB( XtDisplay(w), (wchar_t*)text.ptr, &temp_len ) == NULL ) {
              XtAppWarningMsg( app_con,
		"insertString", "textAction", "XawError",
		"insert-string()'s parameter contents not legal in this locale.",
				NULL, NULL );
              ParameterError( w, *params );
              continue;
          }
      } /* convert to WC */

      if ( _XawTextReplace( ctx, ctx->text.insertPos, 
			    ctx->text.insertPos, &text ) ) {
          XBell( XtDisplay( ctx ), 50 );
          EndAction( ctx );
          return;
      }

      /* Advance insertPos to the end of the string we just inserted. */
      ctx->text.insertPos = SrcScan( ctx->text.source, ctx->text.insertPos,
			    XawstPositions, XawsdRight, text.length, TRUE );

  } /* DO FOR EACH PARAMETER */

  EndAction( ctx );
}


/* DisplayCaret() - action
 *  
 * The parameter list should contain one boolean value.  If the
 * argument is true, the cursor will be displayed.  If false, not.
 *
 * The exception is that EnterNotify and LeaveNotify events may
 * have a second argument, "always".  If they do not, the cursor
 * is only affected if the focus member of the event is true.	*/

static void 
DisplayCaret(w, event, params, num_params)
Widget w;
XEvent* event;		/* CrossingNotify special-cased */
String* params;		/* Off, False, No, On, True, Yes, etc. */
Cardinal* num_params;	/* 0, 1 or 2 */
{
  TextWidget ctx = (TextWidget)w;
  Boolean display_caret = True;

  if  ( ( event->type == EnterNotify || event->type == LeaveNotify ) &&
        ( ( *num_params >= 2 ) && ( strcmp( params[1], "always" ) == 0 ) ) &&
        ( !event->xcrossing.focus ) )
      return;

  if (*num_params > 0) {	/* default arg is "True" */
      XrmValue from, to;
      from.size = strlen(from.addr = params[0]);
      XtConvert( w, XtRString, &from, XtRBoolean, &to );

      if ( to.addr != NULL )
          display_caret = *(Boolean*)to.addr;
      if ( ctx->text.display_caret == display_caret )
          return;
  }
  StartAction(ctx, event);
  ctx->text.display_caret = display_caret;
  EndAction(ctx);
}


/* Multiply() - action
 *  
 * The parameter list may contain either a number or the string 'Reset'.
 * 
 * A number will multiply the current multiplication factor by that number.
 * Many of the text widget actions will will perform n actions, where n is
 * the multiplication factor.
 *
 * The string reset will reset the mutiplication factor to 1. */

/* ARGSUSED */
static void 
Multiply(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
  TextWidget ctx = (TextWidget) w;
  int mult;

  if (*num_params != 1) {
      XtAppError( XtWidgetToApplicationContext( w ), 
	       "Xaw Text Widget: multiply() takes exactly one argument.");
      XBell( XtDisplay( w ), 0 );
      return;
  }

  if ( ( params[0][0] == 'r' ) || ( params[0][0] == 'R' ) ) {
      XBell( XtDisplay( w ), 0 );
      ctx->text.mult = 1;
      return;
  }

  if ( ( mult = atoi( params[0] ) ) == 0 ) {
      char buf[ BUFSIZ ];
      sprintf(buf, "%s %s", "Xaw Text Widget: multiply() argument",
	    "must be a number greater than zero, or 'Reset'." );
      XtAppError( XtWidgetToApplicationContext( w ), buf );
      XBell( XtDisplay( w ), 50 );
      return;
  }

  ctx->text.mult *= mult;
}


/* StripOutOldCRs() - called from FormRegion
 *
 * removes CRs in widget ctx, from from to to.
 *
 * RETURNS: the new ending location (we may add some characters),
 * or XawReplaceError if the widget can't be written to. */

static XawTextPosition
StripOutOldCRs(ctx, from, to)
TextWidget ctx;
XawTextPosition from, to;
{
  XawTextPosition startPos, endPos, eop_begin, eop_end, temp;
  Widget src = ctx->text.source;
  XawTextBlock text;
  char *buf;
  static wchar_t wc_two_spaces[ 3 ];

  /* Initialize our TextBlock with two spaces. */

  text.firstPos = 0;
  text.format = _XawTextFormat(ctx);
  if ( text.format == XawFmt8Bit )
      text.ptr= "  ";
  else {
      wc_two_spaces[0] = _Xaw_atowc(XawSP);
      wc_two_spaces[1] = _Xaw_atowc(XawSP);
      wc_two_spaces[2] = 0;
      text.ptr = (char*) wc_two_spaces;
  }
   
  /* Strip out CR's. */

  eop_begin = eop_end = startPos = endPos = from;
  /* CONSTCOND */
  while (TRUE) {
      endPos=SrcScan(src, startPos, XawstEOL, XawsdRight, 1, FALSE);

      temp = SrcScan(src, endPos, XawstWhiteSpace, XawsdLeft, 1, FALSE);
      temp = SrcScan(src, temp,   XawstWhiteSpace, XawsdRight,1, FALSE);

      if (temp > startPos)
          endPos = temp;

      if (endPos >= to)
          break;

      if (endPos >= eop_begin) {
          startPos = eop_end;
          eop_begin=SrcScan(src, startPos, XawstParagraph, XawsdRight, 1,FALSE);
          eop_end = SrcScan(src, startPos, XawstParagraph, XawsdRight, 1, TRUE);
      }
    else {
      XawTextPosition periodPos, next_word;
      int i, len;

      periodPos= SrcScan(src, endPos, XawstPositions, XawsdLeft, 1, TRUE);
      next_word = SrcScan(src, endPos, XawstWhiteSpace, XawsdRight, 1, FALSE);

      len = next_word - periodPos;

      text.length = 1;
      buf = _XawTextGetText(ctx, periodPos, next_word);
      if (text.format == XawFmtWide) {
        if ( (periodPos < endPos) && (((wchar_t*)buf)[0] == _Xaw_atowc('.')))
          text.length++;
      } else
        if ( (periodPos < endPos) && (buf[0] == '.') )
	  text.length++;	/* Put in two spaces. */

      /*
       * Remove all extra spaces. 
       */

      for (i = 1 ; i < len; i++) 
        if (text.format ==  XawFmtWide) {
          if ( !iswspace(((wchar_t*)buf)[i]) || ((periodPos + i) >= to) ) {
             break;
          }
        } else
	  if ( !isspace(buf[i]) || ((periodPos + i) >= to) ) {
	      break;
	  }
      
      XtFree(buf);

      to -= (i - text.length - 1);
      startPos = SrcScan(src, periodPos, XawstPositions, XawsdRight, i, TRUE);
      if (_XawTextReplace(ctx, endPos, startPos, &text) != XawEditDone)
	  return XawReplaceError;
      startPos -= i - text.length;
    }
  }
  return(to);
}


/* InsertNewCRs() - called from FormRegion
 *
 * inserts new CRs for FormRegion, thus for FormParagraph action */

static void
InsertNewCRs(ctx, from, to)
TextWidget ctx;
XawTextPosition from, to;
{
  XawTextPosition startPos, endPos, space, eol;
  XawTextBlock text;
  int i, width, height, len;
  char * buf;
  static wchar_t wide_CR[ 2 ];

  text.firstPos = 0;
  text.length = 1;
  text.format = _XawTextFormat( ctx );

  if ( text.format == XawFmt8Bit )
      text.ptr = "\n";
  else {
      wide_CR[0] = _Xaw_atowc(XawLF);
      wide_CR[1] = 0;
      text.ptr = (char*) wide_CR;
  }

  startPos = from;
  /* CONSTCOND */
  while (TRUE) {
      XawTextSinkFindPosition( ctx->text.sink, startPos, 
			    (int) ctx->text.margin.left,
			    (int) (ctx->core.width - HMargins(ctx)), 
			    TRUE, &eol, &width, &height);
      if (eol >= to)
          break;

      eol  = SrcScan(ctx->text.source, eol, XawstPositions, XawsdLeft, 1, TRUE);
      space= SrcScan(ctx->text.source, eol, XawstWhiteSpace,XawsdRight,1, TRUE);
    
      startPos = endPos = eol;
      if (eol == space) 
          return;

      len = (int) (space - eol);
      buf = _XawTextGetText(ctx, eol, space);
      for ( i = 0 ; i < len ; i++)
      if (text.format == XawFmtWide) {
          if (!iswspace(((wchar_t*)buf)[i]))
              break;
      } else
          if (!isspace(buf[i]))
              break;

      to -= (i - 1);
      endPos = SrcScan(ctx->text.source, endPos,
		     XawstPositions, XawsdRight, i, TRUE);
      XtFree(buf);
    
      if (_XawTextReplace(ctx, startPos, endPos, &text))
          return;

      startPos = SrcScan(ctx->text.source, startPos,
		       XawstPositions, XawsdRight, 1, TRUE);
  }
}  


/* FormRegion() - called by FormParagraph
 * 
 * oversees the work of paragraph-forming a region
 *
 * RETURNS: XawEditDone if successful, or XawReplaceError. */

static int
FormRegion(ctx, from, to)
TextWidget ctx;
XawTextPosition from, to;
{
  if ( from >= to ) return XawEditDone;

  if ( ( to = StripOutOldCRs( ctx, from, to ) ) == XawReplaceError )
      return XawReplaceError;

  /* insure that the insertion point is within legal bounds */
  if ( ctx->text.insertPos > SrcScan( ctx->text.source, 0,
				       XawstAll, XawsdRight, 1, TRUE ) )
      ctx->text.insertPos = to;

  InsertNewCRs(ctx, from, to);
  _XawTextBuildLineTable(ctx, ctx->text.lt.top, TRUE);
  return XawEditDone;
}


/* FormParagraph() - action
 *
 * removes and reinserts CRs to maximize line length without clipping */

/* ARGSUSED */
static void 
FormParagraph(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
  TextWidget ctx = (TextWidget) w;
  XawTextPosition from, to;

  StartAction(ctx, event);

  from =  SrcScan( ctx->text.source, ctx->text.insertPos,
		  XawstParagraph, XawsdLeft, 1, FALSE );
  to  =  SrcScan( ctx->text.source, from,
		 XawstParagraph, XawsdRight, 1, FALSE );

  if ( FormRegion( ctx, from, to ) == XawReplaceError )
      XBell( XtDisplay( w ), 0 );
  _XawTextSetScrollBars( ctx );
  EndAction( ctx );
}


/* TransposeCharacters() - action
 *
 * Swaps the character to the left of the mark
 * with the character to the right of the mark. */
	     
/* ARGSUSED */
static void 
TransposeCharacters(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
  TextWidget ctx = (TextWidget) w;
  XawTextPosition start, end;
  XawTextBlock text;
  char* buf;
  int i;

  StartAction(ctx, event);

  /* Get bounds. */

  start = SrcScan( ctx->text.source, ctx->text.insertPos, XawstPositions, 
		  XawsdLeft, 1, TRUE );
  end = SrcScan( ctx->text.source, ctx->text.insertPos, XawstPositions, 
		XawsdRight, ctx->text.mult, TRUE );

  /* Make sure we aren't at the very beginning or end of the buffer. */

  if ( ( start == ctx->text.insertPos ) || ( end == ctx->text.insertPos ) ) {
      XBell( XtDisplay( w ), 0 );	/* complain. */
      EndAction( ctx );
      return;
  }

  ctx->text.insertPos = end;

  text.firstPos = 0;
  text.format = _XawTextFormat(ctx);

  /* Retrieve text and swap the characters. */
    
  if ( text.format == XawFmtWide) {
      wchar_t wc;
      wchar_t* wbuf;

      wbuf = (wchar_t*) _XawTextGetText(ctx, start, end);
      text.length = wcslen( wbuf );
      wc = wbuf[ 0 ];
      for ( i = 1; i < text.length; i++ )
          wbuf[ i-1 ] = wbuf[ i ];
      wbuf[ i-1 ] = wc;
      buf = (char*) wbuf; /* so that it gets assigned and freed */

  } else { /* thus text.format == XawFmt8Bit */
      char c;
      buf = _XawTextGetText( ctx, start, end );
      text.length = strlen( buf );
      c = buf[ 0 ];
      for ( i = 1; i < text.length; i++ )
          buf[ i-1 ] = buf[ i ];
      buf[ i-1 ] = c;
  }

  text.ptr = buf;

  /* Store new text in source. */
    
  if (_XawTextReplace (ctx, start, end, &text))	/* Unable to edit, complain. */
      XBell(XtDisplay(w), 0);
  XtFree((char *) buf);
  EndAction(ctx);
}


/* NoOp() - action
 * This action performs no action, and allows the user or
 * application programmer to unbind a translation.
 *
 * Note: If the parameter list contains the string "RingBell" then
 *       this action will ring the bell.
 */

/*ARGSUSED*/
static void
NoOp(w, event, params, num_params)
Widget w;
XEvent* event;
String* params;
Cardinal* num_params;
{
    if (*num_params != 1)
	return;

    switch(params[0][0]) {
    case 'R':
    case 'r':
	XBell(XtDisplay(w), 0);
    default:			/* Fall Through */
	break;
    }
}
	
/* Reconnect() - action
 * This reconnects to the input method.  The user will typically call
 * this action if/when connection has been severed, or when the app
 * was started up before an IM was started up.
 */

/*ARGSUSED*/
static void
Reconnect( w, event, params, num_params )
    Widget w;
    XEvent* event;
    String* params;
    Cardinal* num_params;
{
    _XawImReconnect( w );
}
	

XtActionsRec _XawTextActionsTable[] = {

/* motion bindings */

  {"forward-character", 	MoveForwardChar},
  {"backward-character", 	MoveBackwardChar},
  {"forward-word", 		MoveForwardWord},
  {"backward-word", 		MoveBackwardWord},
  {"forward-paragraph", 	MoveForwardParagraph},
  {"backward-paragraph", 	MoveBackwardParagraph},
  {"beginning-of-line", 	MoveToLineStart},
  {"end-of-line", 		MoveToLineEnd},
  {"next-line", 		MoveNextLine},
  {"previous-line", 		MovePreviousLine},
  {"next-page", 		MoveNextPage},
  {"previous-page", 		MovePreviousPage},
  {"beginning-of-file", 	MoveBeginningOfFile},
  {"end-of-file", 		MoveEndOfFile},
  {"scroll-one-line-up", 	ScrollOneLineUp},
  {"scroll-one-line-down", 	ScrollOneLineDown},

/* delete bindings */

  {"delete-next-character", 	DeleteForwardChar},
  {"delete-previous-character", DeleteBackwardChar},
  {"delete-next-word", 		DeleteForwardWord},
  {"delete-previous-word", 	DeleteBackwardWord},
  {"delete-selection", 		DeleteCurrentSelection},

/* kill bindings */

  {"kill-word", 		KillForwardWord},
  {"backward-kill-word", 	KillBackwardWord},
  {"kill-selection", 		KillCurrentSelection},
  {"kill-to-end-of-line", 	KillToEndOfLine},
  {"kill-to-end-of-paragraph", 	KillToEndOfParagraph},

/* new line stuff */

  {"newline-and-indent", 	InsertNewLineAndIndent},
  {"newline-and-backup", 	InsertNewLineAndBackup},
  {"newline", 			InsertNewLine},

/* Selection stuff */

  {"select-word", 		SelectWord},
  {"select-all", 		SelectAll},
  {"select-start", 		SelectStart},
  {"select-adjust", 		SelectAdjust},
  {"select-end", 		SelectEnd},
  {"select-save",		SelectSave},
  {"extend-start", 		ExtendStart},
  {"extend-adjust", 		ExtendAdjust},
  {"extend-end", 		ExtendEnd},
  {"insert-selection",		InsertSelection},

/* Miscellaneous */

  {"redraw-display", 		RedrawDisplay},
  {"insert-file", 		_XawTextInsertFile},
  {"search",		        _XawTextSearch},
  {"insert-char", 		InsertChar},
  {"insert-string",		InsertString},
  {"focus-in", 	 	        TextFocusIn},
  {"focus-out", 		TextFocusOut},
  {"enter-window", 	 	TextEnterWindow},
  {"leave-window", 		TextLeaveWindow},
  {"display-caret",		DisplayCaret},
  {"multiply",		        Multiply},
  {"form-paragraph",            FormParagraph},
  {"transpose-characters",      TransposeCharacters},
  {"no-op",                     NoOp},

/* Action to bind special translations for text Dialogs. */

  {"InsertFileAction",          _XawTextInsertFileAction},
  {"DoSearchAction",            _XawTextDoSearchAction},
  {"DoReplaceAction",           _XawTextDoReplaceAction},
  {"SetField",                  _XawTextSetField},
  {"PopdownSearchAction",       _XawTextPopdownSearchAction},

/* Reconnect to Input Method */
  {"reconnect-im",       Reconnect} /* Li Yuhong, Omron KK, 1991 */
};

Cardinal _XawTextActionsTableCount = XtNumber(_XawTextActionsTable);
