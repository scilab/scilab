/* $XConsortium: menu.c,v 1.61 92/04/20 18:46:39 rws Exp $ */
/*
Copyright 1989 Massachusetts Institute of Technology

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in advertising or
publicity pertaining to distribution of the software without specific,
written prior permission.  M.I.T. makes no representations about the
suitability of this software for any purpose.  It is provided "as is"
without express or implied warranty.
*/

#include "x_ptyxP.h"
#include "x_data.h"
#include "x_menu.h"
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xmu/CharSet.h>
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/SmeLine.h>
#include <stdio.h>
#include <signal.h>

#include "../machine.h"
#include "All-extern-x.h"


Arg menuArgs[2] = {{ XtNleftBitmap, (XtArgVal) 0 },
		   { XtNsensitive, (XtArgVal) 0 }};


static int domenu  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
static Widget create_menu  __PARAMS((XtermWidget xtw, Widget toplevelw, 
				    char *, struct _MenuEntry *, Cardinal));  
static void handle_send_signal  __PARAMS((Widget gw, int sig));  
static void do_visualbell  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_redraw  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_quit  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_scrollbar  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_jumpscroll  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_reversevideo  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_autowrap  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_reversewrap  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_autolinefeed  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_appcursor  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_appkeypad  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_scrollkey  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_scrollttyoutput  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_allow132  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_cursesemul  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_marginbell  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_altscreen  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_softreset  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_hardreset  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_clearsavedlines  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void do_vtfont  __PARAMS((Widget gw, caddr_t , caddr_t));  
static void handle_toggle __PARAMS(( void(*proc)(),int , String *, 
				   Cardinal , Widget w, caddr_t , caddr_t));  


/*
 * The order entries MUST match the values given in menu.h
 */
MenuEntry mainMenuEntries[] = {
    { "redraw",		do_redraw, NULL }};		/*  3 */


MenuEntry vtMenuEntries[] = {
    { "scrollbar",	do_scrollbar, NULL },		/*  0 */
    { "jumpscroll",	do_jumpscroll, NULL },		/*  1 */
    { "reversevideo",	do_reversevideo, NULL },	/*  2 */
    { "autowrap",	do_autowrap, NULL },		/*  3 */
    { "reversewrap",	do_reversewrap, NULL },		/*  4 */
    { "autolinefeed",	do_autolinefeed, NULL },	/*  5 */
    { "appcursor",	do_appcursor, NULL },		/*  6 */
    { "appkeypad",	do_appkeypad, NULL },		/*  7 */
    { "scrollkey",	do_scrollkey, NULL },		/*  8 */
    { "scrollttyoutput",do_scrollttyoutput, NULL },	/*  9 */
    { "allow132",	do_allow132, NULL },		/* 10 */
    { "cursesemul",	do_cursesemul, NULL },		/* 11 */
    { "visualbell",	do_visualbell, NULL },		/* 12 */
    { "marginbell",	do_marginbell, NULL },		/* 13 */
    { "altscreen",	do_altscreen, NULL },		/* 14 */
    { "line1",		NULL, NULL },			/* 15 */
    { "softreset",	do_softreset, NULL },		/* 16 */
    { "hardreset",	do_hardreset, NULL },		/* 17 */
    { "clearsavedlines",do_clearsavedlines, NULL },	/* 18 */
    { "line2",		NULL, NULL }};			/* 19 */


MenuEntry fontMenuEntries[] = {
    { "fontdefault",	do_vtfont, NULL },		/*  0 */
    { "font1",		do_vtfont, NULL },		/*  1 */
    { "font2",		do_vtfont, NULL },		/*  2 */
    { "font3",		do_vtfont, NULL },		/*  3 */
    { "font4",		do_vtfont, NULL },		/*  4 */
    { "font5",		do_vtfont, NULL },		/*  5 */
    { "font6",		do_vtfont, NULL },		/*  6 */
    { "fontescape",	do_vtfont, NULL },		/*  7 */
    { "fontsel",	do_vtfont, NULL }};		/*  8 */
    /* this should match NMENUFONTS in ptyx.h */


static Widget create_menu();
extern Widget toplevel;


/*
 * we really want to do these dynamically
 */
#define check_width 9
#define check_height 8
static unsigned char check_bits[] = {
   0x00, 0x01, 0x80, 0x01, 0xc0, 0x00, 0x60, 0x00,
   0x31, 0x00, 0x1b, 0x00, 0x0e, 0x00, 0x04, 0x00
};


/*
 * public interfaces
 */


static Bool domenu (w, event, params, param_count)
    Widget w;
    XEvent *event;              /* unused */
    String *params;             /* mainMenu, vtMenu */
    Cardinal *param_count;      /* 0 or 1 */
{
    TScreen *screen = &term->screen;

    if (*param_count != 1) {
	XBell (XtDisplay(w), 0);
	return False;
    }

    switch (params[0][0]) {
      case 'm':
	if (!screen->mainMenu) {
	    screen->mainMenu = create_menu (term, toplevel, "mainMenu",
					    mainMenuEntries,
					    XtNumber(mainMenuEntries));
	}
	break;

      case 'v':
	if (!screen->vtMenu) {
	    screen->vtMenu = create_menu (term, toplevel, "vtMenu",
					  vtMenuEntries,
					  XtNumber(vtMenuEntries));
	    /* and turn off the alternate screen entry */
	    set_altscreen_sensitivity (FALSE);
	    update_scrollbar();
	    update_jumpscroll();
	    update_reversevideo();
	    update_autowrap();
	    update_reversewrap();
	    update_autolinefeed();
	    update_appcursor();
	    update_appkeypad();
	    update_scrollkey();
	    update_scrollttyoutput();
	    update_allow132();
	    update_cursesemul();
	    update_visualbell();
	    update_marginbell();
	}
	break;

      case 'f':
	if (!screen->fontMenu) {
	    screen->fontMenu = create_menu (term, toplevel, "fontMenu",
					    fontMenuEntries,
					    (Cardinal ) NMENUFONTS);  
	    set_menu_font (True);
	    set_sensitivity (screen->fontMenu,
			     fontMenuEntries[fontMenu_fontescape].widget,
			     (screen->menu_font_names[fontMenu_fontescape]
			      ? TRUE : FALSE));
	}
	FindFontSelection ((char *) 0, True);
	set_sensitivity (screen->fontMenu,
			 fontMenuEntries[fontMenu_fontsel].widget,
			 (screen->menu_font_names[fontMenu_fontsel]
			  ? TRUE : FALSE));
	break;

      case 't':
	break;

      default:
	XBell (XtDisplay(w), 0);
	return False;
    }

    return True;
}

void HandleCreateMenu (w, event, params, param_count)
    Widget w;
    XEvent *event;              /* unused */
    String *params;             /* mainMenu, vtMenu */
    Cardinal *param_count;      /* 0 or 1 */
{
    (void) domenu (w, event, params, param_count);
}

void HandlePopupMenu (w, event, params, param_count)
    Widget w;
    XEvent *event;              /* unused */
    String *params;             /* mainMenu, vtMenu */
    Cardinal *param_count;      /* 0 or 1 */
{
    if (domenu (w, event, params, param_count)) {
	XtCallActionProc (w, "XawPositionSimpleMenu", event, params, 1);
	XtCallActionProc (w, "MenuPopup", event, params, 1);
    }
}


/*
 * private interfaces - keep out!
 */

/*
 * create_menu - create a popup shell and stuff the menu into it.
 */

static Widget create_menu (xtw, toplevelw, name, entries, nentries)
     XtermWidget xtw;
     Widget toplevelw;
     char *name;
     struct _MenuEntry *entries;
     Cardinal  nentries;
{
  Widget m;
  TScreen *screen = &xtw->screen;
  static XtCallbackRec cb[2] = { { NULL, NULL }, { NULL, NULL }};
  static Arg arg = { XtNcallback, (XtArgVal) cb };
  
  if (screen->menu_item_bitmap == None) {
	screen->menu_item_bitmap =
	  XCreateBitmapFromData (XtDisplay(xtw),
				 RootWindowOfScreen(XtScreen(xtw)),
				 (char *)check_bits, check_width, check_height);
    }

  m = XtCreatePopupShell (name, simpleMenuWidgetClass, toplevelw, (ArgList) 0,(Cardinal) 0);

    for (; nentries >= (Cardinal) 1; nentries--, entries++) {
	cb[0].callback = (XtCallbackProc) entries->function;
	cb[0].closure = (caddr_t) entries->name;
	entries->widget = XtCreateManagedWidget (entries->name, 
						 (entries->function ?
						  smeBSBObjectClass :
						  smeLineObjectClass), m,
						 &arg, (Cardinal) 1);
    }

    /* do not realize at this point */
    return m;
}


static void handle_send_signal (gw, sig)
    Widget gw;
    int sig;
{
    register TScreen *screen = &term->screen;
    if (screen->pid > 1) kill_process_group ((int) screen->pid, sig);
}



static void do_visualbell (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    screen->visualbell = !screen->visualbell;
    update_visualbell();
}


static void do_redraw (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    Redraw ();
}

/** for signal **/

void do_kill1(i)
     int i;
{
  handle_send_signal (NULL, SIGKILL);
}

void do_kill (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    handle_send_signal (gw, SIGKILL);
}

static void do_quit (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    Cleanup (0);
}



/*
 * vt menu callbacks
 */

static void do_scrollbar (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    if (screen->scrollbar) {
	ScrollBarOff (screen);
    } else {
	ScrollBarOn (term, FALSE, FALSE);
    }
    update_scrollbar();
}


static void do_jumpscroll (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    term->flags ^= SMOOTHSCROLL;
    if (term->flags & SMOOTHSCROLL) {
	screen->jumpscroll = FALSE;
	if (screen->scroll_amt) FlushScroll(screen);
    } else {
	screen->jumpscroll = TRUE;
    }
    update_jumpscroll();
}


static void do_reversevideo (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    term->flags ^= REVERSE_VIDEO;
    ReverseVideo (term);
    /* update_reversevideo done in ReverseVideo */
}


static void do_autowrap (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    term->flags ^= WRAPAROUND;
    update_autowrap();
}


static void do_reversewrap (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    term->flags ^= REVERSEWRAP;
    update_reversewrap();
}


static void do_autolinefeed (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    term->flags ^= LINEFEED;
    update_autolinefeed();
}


static void do_appcursor (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    term->keyboard.flags ^= CURSOR_APL;
    update_appcursor();
}


static void do_appkeypad (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    term->keyboard.flags ^= KYPD_APL;
    update_appkeypad();
}


static void do_scrollkey (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    screen->scrollkey = !screen->scrollkey;
    update_scrollkey();
}


static void do_scrollttyoutput (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    screen->scrollttyoutput = !screen->scrollttyoutput;
    update_scrollttyoutput();
}


static void do_allow132 (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    screen->c132 = !screen->c132;
    update_allow132();
}


static void do_cursesemul (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    screen->curses = !screen->curses;
    update_cursesemul();
}


static void do_marginbell (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    if (!(screen->marginbell = !screen->marginbell)) screen->bellarmed = -1;
    update_marginbell();
}



static void do_altscreen (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    /* do nothing for now; eventually, will want to flip screen */
}


static void do_softreset (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    VTReset (FALSE);
}


static void do_hardreset (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    VTReset (TRUE);
}


static void do_clearsavedlines (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    register TScreen *screen = &term->screen;

    screen->savedlines = 0;
    ScrollBarDrawThumb(screen->scrollWidget);
    VTReset (TRUE); 
}


/*
 * vtfont menu
 */

static void do_vtfont (gw, closure, data)
    Widget gw;
    caddr_t closure, data;
{
    char *entryname = (char *) closure;
    int i;

    for (i = 0; i < NMENUFONTS; i++) {
	if (strcmp (entryname, fontMenuEntries[i].name) == 0) {
	    SetVTFont (i, True, (char *) NULL,(char *)  NULL);
	    return;
	}
    }
    Bell();
}


/*
 * public handler routines
 */

static void handle_toggle (proc, var, params, nparams, w, closure, data)
    void (*proc)();
    int var;
    String *params;
    Cardinal nparams;
    Widget w;
    caddr_t closure, data;
{
    int dir = -2;

    switch (nparams) {
      case 0:
	dir = -1;
	break;
      case 1:
	if (XmuCompareISOLatin1 (params[0], "on") == 0) dir = 1;
	else if (XmuCompareISOLatin1 (params[0], "off") == 0) dir = 0;
	else if (XmuCompareISOLatin1 (params[0], "toggle") == 0) dir = -1;
	break;
    }

    switch (dir) {
      case -2:
	Bell();
	break;

      case -1:
	(*proc) (w, closure, data);
	break;

      case 0:
	if (var) (*proc) (w, closure, data);
	else Bell();
	break;

      case 1:
	if (!var) (*proc) (w, closure, data);
	else Bell();
	break;
    }
    return;
}


void HandleSetVisualBell(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_visualbell, (int) term->screen.visualbell,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}



void HandleRedraw(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    do_redraw(w,(caddr_t) NULL,(caddr_t) NULL);
}


void HandleSendSignal(w, event, params, param_count)
    Widget w;
    XEvent *event;		/* unused */
    String *params;
    Cardinal *param_count;
{
    static struct sigtab {
	char *name;
	int sig;
    } signals[] = {
#ifdef SIGTSTP
	{ "suspend",	SIGTSTP },
	{ "tstp",	SIGTSTP },
#endif
#ifdef SIGCONT
	{ "cont",	SIGCONT },
#endif
	{ "int",	SIGINT },
	{ "hup",	SIGHUP },
	{ "quit",	SIGQUIT },
	{ "alrm",	SIGALRM },
	{ "alarm",	SIGALRM },
	{ "term",	SIGTERM },
	{ "kill",	SIGKILL },
	{ NULL, 0 },
    };

    if (*param_count == 1) {
	struct sigtab *st;

	for (st = signals; st->name; st++) {
	    if (XmuCompareISOLatin1 (st->name, params[0]) == 0) {
		handle_send_signal (w, st->sig);
		return;
	    }
	}
	/* one could allow numeric values, but that would be a security hole */
    }

    Bell();
}


void HandleQuit(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    do_quit(w,(caddr_t) NULL,(caddr_t) NULL);
}

void HandleScrollbar(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_scrollbar, (int) term->screen.scrollbar,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleJumpscroll(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_jumpscroll, (int) term->screen.jumpscroll,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleReverseVideo(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_reversevideo, (int) (term->flags & REVERSE_VIDEO),
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleAutoWrap(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_autowrap, (int) (term->flags & WRAPAROUND),
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleReverseWrap(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_reversewrap, (int) (term->flags & REVERSEWRAP),
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleAutoLineFeed(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_autolinefeed, (int) (term->flags & LINEFEED),
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleAppCursor(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_appcursor, (int) (term->keyboard.flags & CURSOR_APL),
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleAppKeypad(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_appkeypad, (int) (term->keyboard.flags & KYPD_APL),
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleScrollKey(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_scrollkey, (int) term->screen.scrollkey,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleScrollTtyOutput(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_scrollttyoutput, (int) term->screen.scrollttyoutput,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleAllow132(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_allow132, (int) term->screen.c132,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleCursesEmul(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_cursesemul, (int) term->screen.curses,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleMarginBell(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    handle_toggle (do_marginbell, (int) term->screen.marginbell,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}

void HandleAltScreen(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    /* eventually want to see if sensitive or not */
    handle_toggle (do_altscreen, (int) term->screen.alternate,
		   params, *param_count, w, (caddr_t) NULL, (caddr_t) NULL);
}


void HandleSoftReset(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    do_softreset(w,(caddr_t) NULL,(caddr_t) NULL);
}


void HandleHardReset(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    do_hardreset(w,(caddr_t) NULL,(caddr_t) NULL);
}


void HandleClearSavedLines(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    do_clearsavedlines(w,(caddr_t) NULL, (caddr_t) NULL);
}

