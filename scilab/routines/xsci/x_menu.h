/* $XConsortium: menu.h,v 1.23 91/06/25 19:49:44 gildea Exp $ */

/* Copyright 1989 Massachusetts Institute of Technology */

/*
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

typedef struct _MenuEntry {
    char *name;
    void (*function)();
    Widget widget;
} MenuEntry;

extern MenuEntry mainMenuEntries[], vtMenuEntries[];
extern MenuEntry fontMenuEntries[];
extern Arg menuArgs[];


extern void HandleSetVisualBell();
extern void HandleRedraw();
extern void HandleSendSignal();
extern void HandleQuit();
extern void HandleScrollbar();
extern void HandleJumpscroll();
extern void HandleReverseVideo();
extern void HandleAutoWrap();
extern void HandleReverseWrap();
extern void HandleAutoLineFeed();
extern void HandleAppCursor();
extern void HandleAppKeypad();
extern void HandleScrollKey();
extern void HandleScrollTtyOutput();
extern void HandleAllow132();
extern void HandleCursesEmul();
extern void HandleMarginBell();
extern void HandleAltScreen();
extern void HandleSoftReset();
extern void HandleHardReset();
extern void HandleClearSavedLines();
extern void HandleVisibility();

/*
 * The following definitions MUST match the order of entries given in 
 * the mainMenuEntries, vtMenuEntries arrays in menu.c.
 */

/*
 * items in primary menu
 */

#define mainMenu_redraw 0

/*
 * items in vt100 mode menu
 */

#define vtMenu_scrollbar 0
#define vtMenu_jumpscroll 1
#define vtMenu_reversevideo 2
#define vtMenu_autowrap 3
#define vtMenu_reversewrap 4
#define vtMenu_autolinefeed 5
#define vtMenu_appcursor 6
#define vtMenu_appkeypad 7
#define vtMenu_scrollkey 8
#define vtMenu_scrollttyoutput 9
#define vtMenu_allow132 10
#define vtMenu_cursesemul 11
#define vtMenu_visualbell 12
#define vtMenu_marginbell 13
#define vtMenu_altscreen 14
#define vtMenu_line1 15
#define vtMenu_softreset 16
#define vtMenu_hardreset 17
#define vtMenu_clearsavedlines 18
#define vtMenu_line2 19

/*
 * items in vt100 font menu
 */
#define fontMenu_fontdefault 0
#define fontMenu_font1 1
#define fontMenu_font2 2
#define fontMenu_font3 3
#define fontMenu_font4 4
#define fontMenu_font5 5
#define fontMenu_font6 6
#define fontMenu_lastBuiltin fontMenu_font6
#define fontMenu_fontescape 7
#define fontMenu_fontsel 8
/* number of non-line items should match NMENUFONTS in ptyx.h */



/*
 * macros for updating menus
 */

#define update_menu_item(w,mi,val) { if (mi) { \
    menuArgs[0].value = (XtArgVal) ((val) ? term->screen.menu_item_bitmap \
				          : None); \
    XtSetValues (mi, menuArgs, (Cardinal) 1); }}


#define set_sensitivity(w,mi,val) { if (mi) { \
    menuArgs[1].value = (XtArgVal) (val); \
    XtSetValues (mi, menuArgs+1, (Cardinal) 1);  }}



/*
 * there should be one of each of the following for each checkable item
 */


#define update_scrollbar() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_scrollbar].widget, \
		    term->screen.scrollbar)

#define update_jumpscroll() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_jumpscroll].widget, \
		    term->screen.jumpscroll)

#define update_reversevideo() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_reversevideo].widget, \
		    (term->flags & REVERSE_VIDEO))

#define update_autowrap() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_autowrap].widget, \
		    (term->flags & WRAPAROUND))

#define update_reversewrap() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_reversewrap].widget, \
		    (term->flags & REVERSEWRAP))

#define update_autolinefeed() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_autolinefeed].widget, \
		    (term->flags & LINEFEED))

#define update_appcursor() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_appcursor].widget, \
		    (term->keyboard.flags & CURSOR_APL))

#define update_appkeypad() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_appkeypad].widget, \
		    (term->keyboard.flags & KYPD_APL))

#define update_scrollkey() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_scrollkey].widget,  \
		    term->screen.scrollkey)

#define update_scrollttyoutput() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_scrollttyoutput].widget, \
		    term->screen.scrollttyoutput)

#define update_allow132() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_allow132].widget, \
		    term->screen.c132)
  
#define update_cursesemul() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_cursesemul].widget, \
		    term->screen.curses)

#define update_visualbell() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_visualbell].widget, \
		    term->screen.visualbell)

#define update_marginbell() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_marginbell].widget, \
		    term->screen.marginbell)

#define update_altscreen() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_altscreen].widget, \
		    term->screen.alternate)


#define set_altscreen_sensitivity(val) \
  set_sensitivity (term->screen.vtMenu,\
		   vtMenuEntries[vtMenu_altscreen].widget, (val))


#define set_menu_font(val) \
  update_menu_item (term->screen.fontMenu, \
		    fontMenuEntries[term->screen.menu_font_number].widget, \
		    (val))
