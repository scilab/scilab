/*
 *	$XConsortium: misc.c,v 1.92 92/03/13 17:02:08 gildea Exp $
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

#include "x_ptyxP.h"	/* X headers included here. */
#include "../machine.h"

#include <X11/Xos.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <pwd.h>
#include <errno.h>

#include <X11/Xatom.h>
#include <X11/cursorfont.h>

#include <X11/Shell.h>
#include <X11/Xmu/Error.h>
#include <X11/Xmu/SysUtil.h>
#include <X11/Xmu/WinUtil.h>

#include "x_data.h"
#include "x_error.h"
#include "x_menu.h"

#include "All-extern-x.h"

#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#else
extern char *malloc();
extern char *getenv();
#endif
#if defined(macII) && !defined(__STDC__)	/* stdlib.h fails to define
						 * these */
char *malloc();
#endif	 /* macII */

static void DoSpecialEnterNotify();
static void DoSpecialLeaveNotify();

void x_events()
{
  XEvent event;
  register TScreen *screen = &term->screen;
  if (screen->scroll_amt)
    FlushScroll(screen);
  if (!XPending(screen->display))
    /* protect against events/errors being swallowed by us or Xlib */
    {
      return;
    }
  do
  {
    if (waitingForTrackInfo)
      {
	return;
      }
    XNextEvent(screen->display, &event);
    /* Hack to get around problems with the toolkit throwing away eventing
     * during the exclusive grab of the menu popup.  By looking at the event
     * ourselves we make sure that we can do the right thing. */
    if (event.type == EnterNotify &&
	(event.xcrossing.window == XtWindow(XtParent(term))))
      DoSpecialEnterNotify(&event.xcrossing);
    else if (event.type == LeaveNotify &&
	     (event.xcrossing.window == XtWindow(XtParent(term))))
      DoSpecialLeaveNotify(&event.xcrossing);
    if (!event.xany.send_event ||
	screen->allowSendEvents ||
	((event.xany.type != KeyPress) &&
	 (event.xany.type != KeyRelease) &&
	 (event.xany.type != ButtonPress) &&
	 (event.xany.type != ButtonRelease)))
      {
	XtDispatchEvent(&event);
      }
  } while (QLength(screen->display) > 0);
}

Cursor make_colored_cursor(cursorindex, fg, bg)
  int cursorindex;	/* index into font */
  unsigned long fg, bg;	/* pixel value */
{
  register TScreen *screen = &term->screen;
  Cursor c;
  register Display *dpy = screen->display;

  c = XCreateFontCursor(dpy, cursorindex);
  if (c == (Cursor) 0)
    return (c);

  recolor_cursor(c, fg, bg);
  return (c);
}

/* ARGSUSED */
void HandleKeyPressed(w, event, params, nparams)
  Widget w;
  XEvent *event;
  String *params;
  Cardinal *nparams;
{
  register TScreen *screen = &term->screen;

#ifdef ACTIVEWINDOWINPUTONLY
  if (w == (Widget) term)
#endif
    Input(&term->keyboard, screen, &event->xkey, False);
}
/* ARGSUSED */
void HandleEightBitKeyPressed(w, event, params, nparams)
  Widget w;
  XEvent *event;
  String *params;
  Cardinal *nparams;
{
  register TScreen *screen = &term->screen;

#ifdef ACTIVEWINDOWINPUTONLY
  if (w == (Widget) term)
#endif
    Input(&term->keyboard, screen, &event->xkey, True);
}

/* ARGSUSED */
void HandleStringEvent(w, event, params, nparams)
  Widget w;
  XEvent *event;
  String *params;
  Cardinal *nparams;
{
  register TScreen *screen = &term->screen;

#ifdef ACTIVEWINDOWINPUTONLY
  if (w != (Widget) term)
    return;
#endif

  if (*nparams != 1)
    return;

  if ((*params)[0] == '0' && (*params)[1] == 'x' && (*params)[2] != '\0')
  {
    char c, *p, hexval[2];
    hexval[0] = hexval[1] = 0;
    for (p = *params + 2; (c = *p); p++)
    {
      hexval[0] *= 16;
      if (isupper(c))
	c = tolower(c);
      if (c >= '0' && c <= '9')
	hexval[0] += c - '0';
      else if (c >= 'a' && c <= 'f')
	hexval[0] += c - 'a' + 10;
      else
	break;
    }
    if (c == '\0')
      StringInput(screen, hexval, 1);
  } else
  {
    StringInput(screen, *params, strlen(*params));
  }
}

static void DoSpecialEnterNotify(ev)
  register XEnterWindowEvent *ev;
{
  register TScreen *screen = &term->screen;

#ifdef ACTIVEWINDOWINPUTONLY
  if (ev->window == XtWindow(XtParent((Widget) term)))
#endif
    if (((ev->detail) != NotifyInferior) &&
	ev->focus &&
	!(screen->select & FOCUS))
      selectwindow(screen, INWINDOW);
}

/*ARGSUSED*/
void HandleEnterWindow(w, eventdata, event)
  Widget w;
  register XEnterWindowEvent *event;
  caddr_t eventdata;
{
  /* NOP since we handled it above */
}


static void DoSpecialLeaveNotify(ev)
  register XEnterWindowEvent *ev;
{
  register TScreen *screen = &term->screen;

#ifdef ACTIVEWINDOWINPUTONLY
  if (ev->window == XtWindow(XtParent((Widget) term)))
#endif
    if (((ev->detail) != NotifyInferior) &&
	ev->focus &&
	!(screen->select & FOCUS))
      unselectwindow(screen, INWINDOW);
}


/*ARGSUSED*/
void HandleLeaveWindow(w, eventdata, event)
  Widget w;
  register XEnterWindowEvent *event;
  caddr_t eventdata;
{
  /* NOP since we handled it above */
}


/*ARGSUSED*/
void HandleFocusChange(w, eventdata, event)
  Widget w;
  register XFocusChangeEvent *event;
  caddr_t eventdata;
{
  register TScreen *screen = &term->screen;

  if (event->type == FocusIn)
    selectwindow(screen,
		 (event->detail == NotifyPointer) ? INWINDOW :
		 FOCUS);
  else
  {
    unselectwindow(screen,
		   (event->detail == NotifyPointer) ? INWINDOW :
		   FOCUS);
    if (screen->grabbedKbd && (event->mode == NotifyUngrab))
    {
      XBell(screen->display, 100);
      ReverseVideo(term);
      screen->grabbedKbd = FALSE;
    }
  }
}



void selectwindow(screen, flag)
  register TScreen *screen;
  register int flag;
{
  if (screen->cursor_state &&
      (screen->cursor_col != screen->cur_col ||
       screen->cursor_row != screen->cur_row))
    HideCursor();
  screen->select |= flag;
  if (screen->cursor_state)
    ShowCursor();
  return;
}

void unselectwindow(screen, flag)
  register TScreen *screen;
  register int flag;
{
  if (screen->always_highlight)
    return;
  screen->select &= ~flag;
  if (screen->cursor_state &&
      (screen->cursor_col != screen->cur_col ||
       screen->cursor_row != screen->cur_row))
    HideCursor();
  if (screen->cursor_state)
    ShowCursor();
}

static long lastBellTime;	/* in milliseconds */

void Bell()
{
  extern XtermWidget term;
  register TScreen *screen = &term->screen;
  struct timeval curtime;
  long now_msecs;

  /* has enough time gone by that we are allowed to ring the bell again? */
  if (screen->bellSuppressTime)
  {
    if (screen->bellInProgress)
    {
      if (QLength(screen->display) > 0 ||
	  GetBytesAvailable(ConnectionNumber(screen->display)) > 0)
	x_events();
      if (screen->bellInProgress)
      {				/* even after new events? */
	return;
      }
    }
    gettimeofday(&curtime, NULL);
    now_msecs = 1000 * curtime.tv_sec + curtime.tv_usec / 1000;
    if (lastBellTime != 0 && now_msecs - lastBellTime >= 0 &&
	now_msecs - lastBellTime < screen->bellSuppressTime)
    {
      return;
    }
    lastBellTime = now_msecs;
  }
  if (screen->visualbell)
    VisualBell();
  else
    XBell(screen->display, 0);

  if (screen->bellSuppressTime)
  {
    /* now we change a property and wait for the notify event to come back.
     * If the server is suspending operations while the bell is being emitted
     * (problematic for audio bell), this lets us know when the previous bell
     * has finished */
    Widget w = (Widget) term;
    XChangeProperty(XtDisplay(w), XtWindow(w),
		    XA_NOTICE, XA_NOTICE, 8, PropModeAppend,(unsigned char*) NULL, 0);
    screen->bellInProgress = TRUE;
  }
}


void VisualBell()
{
  extern XtermWidget term;
  register TScreen *screen = &term->screen;
  register Pixel xorPixel = screen->foreground ^ term->core.background_pixel;
  XGCValues gcval;
  GC  visualGC;

  gcval.function = GXxor;
  gcval.foreground = xorPixel;
  visualGC = XtGetGC((Widget) term, GCFunction + GCForeground, &gcval);
  XFillRectangle(
		 screen->display,
		 VWindow(screen),
		 visualGC,
		 0, 0,
		 (unsigned) FullWidth(screen),
		 (unsigned) FullHeight(screen));
  XFlush(screen->display);
  XFillRectangle(
		 screen->display,
		 VWindow(screen),
		 visualGC,
		 0, 0,
		 (unsigned) FullWidth(screen),
		 (unsigned) FullHeight(screen));
}

/* ARGSUSED */
void HandleBellPropertyChange(w, data, ev, more)
  Widget w;
  XtPointer data;
  XEvent *ev;
  Boolean *more;
{
  register TScreen *screen = &term->screen;

  if (ev->xproperty.atom == XA_NOTICE)
  {
    screen->bellInProgress = FALSE;
  }
}

void Redraw()
{
  extern XtermWidget term;
  register TScreen *screen = &term->screen;
  XExposeEvent event;

  event.type = Expose;
  event.display = screen->display;
  event.x = 0;
  event.y = 0;
  event.count = 0;

  if (VWindow(screen))
  {
    event.window = VWindow(screen);
    event.width = term->core.width;
    event.height = term->core.height;
    (*term->core.widget_class->core_class.expose)
      ((Widget) term, (XEvent *) & event, NULL);
    if (screen->scrollbar)
      (*screen->scrollWidget->core.widget_class->core_class.expose)
	(screen->scrollWidget, (XEvent *) & event, NULL);
  }
}

void do_osc(func)
  int (*func) ();
{
  register int mode, c;
  register char *cp;
  char buf[512];
  char *bufend = &buf[(sizeof buf) - 1];	/* leave room for null */
  Bool okay = True;

  /* lines should be of the form <ESC> ] number ; string <BEL>
   * 
   * where number is one of 0, 1, 2, or 46 */
  mode = 0;
  while (isdigit(c = (*func) ()))
    mode = 10 * mode + (c - '0');
  if (c != ';')
    okay = False;
  cp = buf;
  while (isprint((c = (*func) ()) & 0x7f) && cp < bufend)
    *cp++ = c;
  if (c != 7)
    okay = False;
  *cp = 0;
  if (okay)
    switch (mode)
    {
    case 0:			/* new icon name and title */
      Changename(buf);
      Changetitle(buf);
      break;

    case 1:			/* new icon name only */
      Changename(buf);
      break;

    case 2:			/* new title only */
      Changetitle(buf);
      break;

    case 46:			/* ignore log file */
      Bell();
      break;

    case 50:
      SetVTFont(fontMenu_fontescape, True, buf, (char *)0);
      break;

      /* One could write code to send back the display and host names, but
       * that could potentially open a fairly nasty security hole. */
    }
}

static void ChangeGroup(attribute, value)
  String attribute;
  XtArgVal value;
{
  extern Widget toplevel;
  Arg args[1];

  XtSetArg(args[0], attribute, value);
  XtSetValues(toplevel, args, 1);
}

void Changename(name)
  register char *name;
{
  ChangeGroup(XtNiconName, (XtArgVal) name);
}

void Changetitle(name)
  register char *name;
{
  ChangeGroup(XtNtitle, (XtArgVal) name);
}

#ifndef DEBUG
/* ARGSUSED */
#endif
void Panic(s, a)
  char *s;
  int a;
{
#ifdef DEBUG
  if (debug)
  {
    fprintf(stderr, "%s: PANIC!	", xterm_name);
    fprintf(stderr, s, a);
    fputs("\r\n", stderr);
    fflush(stderr);
  }
#endif				/* DEBUG */
}

/** copied from wf_fig.h **/

/* #include "wf_fig.h" */ /** for sys_errlist **/
#ifndef linux 
#if !defined(__bsdi__) && !defined(__NetBSD__) && !defined(__APPLE__) && !defined(__FreeBSD__)
extern int	errno;
extern int	sys_nerr;
#if (! (defined(BSD) && (BSD >= 199306))) && !defined(__FreeBSD__)  && !defined(__APPLE__)
extern char    *sys_errlist[];
#endif
#endif
#endif 

static char UE[]="unknown error";

const char *SysErrorMsg(n)
  int n;
{
  return ((n >= 0 && n < sys_nerr) ? sys_errlist[n] : UE );
}


void SysError(i)
  int i;
{
  int oerrno;

  oerrno = errno;
  /* perror(3) write(2)s to file descriptor 2 */
  fprintf(stderr, "%s: Error %d, errno %d: ", xterm_name, i, oerrno);
  fprintf(stderr, "%s\n", SysErrorMsg(oerrno));
  Cleanup(i);
}

void Error(i)
  int i;
{
  fprintf(stderr, "%s: Error %d\n", xterm_name, i);
  Cleanup(i);
}


/*
 * cleanup by sending SIGHUP to client processes
 */

void Cleanup(code)
  int code;
{
  extern XtermWidget term;
  register TScreen *screen;

  screen = &term->screen;
  if (screen->pid > 1)
  {
    (void) kill_process_group((int) screen->pid, SIGHUP);
  }
 sci_clear_and_exit(code);
}



/*
 * returns a pointer to the first occurrence of s2 in s1,
 * or NULL if there are none.
 */
char *strindex(s1, s2)
  register char *s1, *s2;
{
  register char *s3;
  int s2len = strlen(s2);

  while ((s3 = index(s1, *s2)) != NULL)
  {
    if (strncmp(s3, s2, s2len) == 0)
      return (s3);
    s1 = ++s3;
  }
  return (NULL);
}

/*ARGSUSED*/
void xerror(d, ev)
  Display *d;
  register XErrorEvent *ev;
{
 #ifndef NDEBUG
  fprintf(stderr, "%s:  warning, error event receieved:\n", xterm_name);
  (void) XmuPrintDefaultErrorMessage(d, ev, stderr);
  /*sci_clear_and_exit(ERROR_XERROR);*/
  #endif
}

/*ARGSUSED*/
void xioerror(dpy)
  Display *dpy;
{
  (void)
    fprintf(stderr,
	    "%s:  fatal IO error %d (%s) or KillClient on X server \"%s\"\r\n",
	    xterm_name, errno, SysErrorMsg(errno),
	    DisplayString(dpy));
 sci_clear_and_exit(ERROR_XIOERROR);
}

/**
static void withdraw_window(dpy, w, scr)
  Display *dpy;
  Window w;
  int scr;
{
  (void) XmuUpdateMapHints(dpy, w, (XSizeHints *)0);
  XWithdrawWindow(dpy, w, scr);
  return;
}
**/

void set_vt_visibility(on)
  Boolean on;
{
  extern Widget toplevel;	/* JPC */
  register TScreen *screen = &term->screen;
  if (on)
  {
    if (!screen->Vshow && term)
    {
      VTInit1(toplevel);	/* JPC */
      XtMapWidget(term->core.parent);
      screen->Vshow = TRUE;
    }
  } 
}
