/* Copyright (C) 1998 Chancelier Jean-Philippe */
#include <stdio.h>
#include <math.h>
#include "../graphics/Math.h"

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Cardinals.h>

typedef  struct  {
  char *name;
  void  (*fonc)();} TestOpTab ;

extern TestOpTab GraphicTestTab[];

extern XtAppContext app_con;

/*	Function Name: MenuSelect
 *	Description: called whenever a menu item is selected.
 *	Arguments: w - the menu item that was selected.
 *                 junk, garbage - *** unused ***.
 *	Returns: 
 */

#define streq(a, b)        ( strcmp((a), (b)) == 0 )

/* ARGSUSED */
static void
MenuSelect(w, junk, garbage)
     Widget w;
     XtPointer junk;
     XtPointer garbage;
{
  static firstentry=0;
  integer i;
  if (firstentry==0) 
    {
      integer verbose=0,ww,narg;
      C2F(SetDriver)("Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      C2F(dr)("xget","window",&verbose,&ww,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","window",&ww,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      firstentry++;
    }
  else 
    {
      integer num=0;
      C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xstart","v",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  i=0;
  while ( GraphicTestTab[i].name != (char *) NULL)
     {
       if (streq(XtName(w), GraphicTestTab[i].name)) 
	 {
	   (*(GraphicTestTab[i].fonc))();
	 }
       i++;
     }
}



/*
 * AddNewMenu 
 */

void AddNewMenu(parent, drawbox)
     Widget parent;
     Widget drawbox;
{
  Widget button,menu,entry;
  static Arg args[1] ;
  Cardinal n=0;
  int i=0;
  XtSetArg(args[n], XtNfromHoriz,drawbox ); n++;
  button = XtCreateManagedWidget("menuButton", menuButtonWidgetClass, parent,
				 args,n);
  n=0;
  menu = XtCreatePopupShell("menu", simpleMenuWidgetClass, button,args,n);
  while ( GraphicTestTab[i].name != (char *) NULL)
     {
       entry= XtCreateManagedWidget(GraphicTestTab[i].name, smeBSBObjectClass, menu,
				    args, ZERO);
       XtAddCallback(entry,XtNcallback,MenuSelect,(XtPointer) NULL);
       i++;
     }
}


