/* $XConsortium: MenuButton.c,v 1.21 95/06/26 20:35:12 kaleb Exp $ */

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
 *
 */


/***********************************************************************
 *
 * MenuButton Widget
 *
 ***********************************************************************/

/*
 * MenuButton.c - Source code for MenuButton widget.
 *
 * This is the source code for the Athena MenuButton widget.
 * It is intended to provide an easy method of activating pulldown menus.
 *
 * Date:    May 2, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium 
 *          kit@expo.lcs.mit.edu
 */

#include <stdio.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/MenuButtoP.h>

static void ClassInitialize();
static void PopupMenu();

#define superclass ((CommandWidgetClass)&commandClassRec)

static char defaultTranslations[] = 
"<EnterWindow>: highlight()\n\
 <LeaveWindow>: reset()\n\
 Any<BtnDown>:  reset() PopupMenu()";

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

#define offset(field) XtOffsetOf(MenuButtonRec, field)
static XtResource resources[] = {
  {
    XtNmenuName, XtCMenuName, XtRString, sizeof(String), 
    offset(menu_button.menu_name), XtRString, (XtPointer)"menu"},
};
#undef offset

static XtActionsRec actionsList[] =
{
  {"PopupMenu",	PopupMenu}
};

MenuButtonClassRec menuButtonClassRec = {
  {
    (WidgetClass) superclass,		/* superclass		  */	
    "MenuButton",			/* class_name		  */
    sizeof(MenuButtonRec),       	/* size			  */
    ClassInitialize,			/* class_initialize	  */
    NULL,				/* class_part_initialize  */
    FALSE,				/* class_inited		  */
    NULL,				/* initialize		  */
    NULL,				/* initialize_hook	  */
    XtInheritRealize,			/* realize		  */
    actionsList,			/* actions		  */
    XtNumber(actionsList),		/* num_actions		  */
    resources,				/* resources		  */
    XtNumber(resources),		/* resource_count	  */
    NULLQUARK,				/* xrm_class		  */
    FALSE,				/* compress_motion	  */
    TRUE,				/* compress_exposure	  */
    TRUE,				/* compress_enterleave    */
    FALSE,				/* visible_interest	  */
    NULL,				/* destroy		  */
    XtInheritResize,			/* resize		  */
    XtInheritExpose,			/* expose		  */
    NULL,				/* set_values		  */
    NULL,				/* set_values_hook	  */
    XtInheritSetValuesAlmost,		/* set_values_almost	  */
    NULL,				/* get_values_hook	  */
    NULL,				/* accept_focus		  */
    XtVersion,				/* version		  */
    NULL,				/* callback_private	  */
    defaultTranslations,               	/* tm_table		  */
    XtInheritQueryGeometry,		/* query_geometry	  */
    XtInheritDisplayAccelerator,	/* display_accelerator	  */
    NULL				/* extension		  */
  },  /* CoreClass fields initialization */
  {
    XtInheritChangeSensitive		/* change_sensitive	  */ 
  },  /* SimpleClass fields initialization */
  {
    XtInheritXaw3dShadowDraw,           /* shadowdraw           */
  },  /* ThreeDClass fields initialization */
  {
    0,                                     /* field not used    */
  },  /* LabelClass fields initialization */
  {
    0,                                     /* field not used    */
  },  /* CommandClass fields initialization */
  {
    0,                                     /* field not used    */
  }  /* MenuButtonClass fields initialization */
};

  /* for public consumption */
WidgetClass menuButtonWidgetClass = (WidgetClass) &menuButtonClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static void ClassInitialize()
{
    XawInitializeWidgetSet();
    XtRegisterGrabAction(PopupMenu, True, 
			 (unsigned int)(ButtonPressMask | ButtonReleaseMask),
			 GrabModeAsync, GrabModeAsync);
}

/* ARGSUSED */
static void
PopupMenu(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal * num_params;
{
  MenuButtonWidget mbw = (MenuButtonWidget) w;
  Widget menu = NULL, temp;
  Arg arglist[2];
  Cardinal num_args;
  int menu_x, menu_y, menu_width, menu_height, button_height;
  Position button_x, button_y;

  temp = w;
  while(temp != NULL) {
    menu = XtNameToWidget(temp, mbw->menu_button.menu_name);
    if (menu == NULL) 
      temp = XtParent(temp);
    else
      break;
  }

  if (menu == NULL) {
    char error_buf[BUFSIZ];
    (void) sprintf(error_buf, "MenuButton: %s %s.",
	    "Could not find menu widget named", mbw->menu_button.menu_name);
    XtAppWarning(XtWidgetToApplicationContext(w), error_buf);
    return;
  }
  if (!XtIsRealized(menu))
    XtRealizeWidget(menu);
  
  menu_width = menu->core.width + 2 * menu->core.border_width;
  button_height = w->core.height + 2 * w->core.border_width;
  menu_height = menu->core.height + 2 * menu->core.border_width;

  XtTranslateCoords(w, 0, 0, &button_x, &button_y);
  menu_x = button_x;
  menu_y = button_y + button_height;

  if (menu_x >= 0) {
    int scr_width = WidthOfScreen(XtScreen(menu));
    if (menu_x + menu_width > scr_width)
      menu_x = scr_width - menu_width;
  }
  if (menu_x < 0) 
    menu_x = 0;

  if (menu_y >= 0) {
    int scr_height = HeightOfScreen(XtScreen(menu));
    if (menu_y + menu_height > scr_height)
      menu_y = scr_height - menu_height;
  }
  if (menu_y < 0)
    menu_y = 0;

  num_args = 0;
  XtSetArg(arglist[num_args], XtNx, menu_x); num_args++;
  XtSetArg(arglist[num_args], XtNy, menu_y); num_args++;
  XtSetValues(menu, arglist, num_args);

  XtPopupSpringLoaded(menu);
}

