/*------------------------------------------------------------------------
    Graphic library for 2D and 3D plotting 
    Copyright (C) 1998 Chancelier Jean-Philippe
    jpc@cergrene.enpc.fr 
 --------------------------------------------------------------------------*/

#include "machine.h"
#include <stdio.h>
extern	WindowPtr CWindow;
static int width=0;

MenuHandle	appleMenu, fileMenu, editMenu, widthMenu;

enum	{
	appleID = 1,
	fileID,
	widthID
	};

enum	{
	quitItem = 1
	};

SetUpMenus()

{
	InsertMenu(appleMenu = NewMenu(appleID, "\p\024"), 0);
	InsertMenu(fileMenu = NewMenu(fileID, "\pFile"), 0);
	InsertMenu(widthMenu = NewMenu(widthID, "\ptest"), 0);
	DrawMenuBar();
	AddResMenu(appleMenu, 'DRVR');
	AppendMenu(fileMenu, "\pQuit/Q");
	AppendMenu(widthMenu, "\p1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21");
}


AdjustMenus()
{
	register WindowPeek wp = (WindowPeek) FrontWindow();
	short kind = wp ? wp->windowKind : 0;
	Boolean DA = kind < 0;
	CheckItem(widthMenu, width, true);
	enable(editMenu, 1, 1);

}


static
enable(menu, item, ok)
Handle menu;
{
	if (ok)
		EnableItem(menu, item);
	else
		DisableItem(menu, item);
}

#define PI0 (integer *) 0
#define PD0 (double *) 0

CleanWinTape()
{
  integer verbose=0,ww=0,narg;
  C2F(dr)("xget","window",&verbose,&ww,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); 
  C2F(dr)("xset","window",&ww,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xstart","v",&ww,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
 }


HandleMenu (mSelect)

	long	mSelect;

{
	int			menuID = HiWord(mSelect);
	int			menuItem = LoWord(mSelect);
	Str255		name;
	GrafPtr		savePort;
	WindowPeek	frontWindow;
	
	switch (menuID)
	  {
	  case	appleID:
		GetPort(&savePort);
		GetItem(appleMenu, menuItem, name);
		OpenDeskAcc(name);
		SetPort(savePort);
		break;
	
	  case	fileID:
		switch (menuItem)
		  {
		  case	quitItem:
		    Scistring(" exit seelected \n");
			ExitToShell();
			break;
		  }
		break;

	  case	widthID:
		CheckItem(widthMenu, width, false);
		width = menuItem;
		CleanWinTape();
		UpdateCWindow();
		switch (menuItem)
		  {
		  case	1:
		    test2D();
		    break;
		  case 2:
		    test2D2();
		    break;
		  case 3:
		    test2D3();
		    break;
		  case 4:
		    test2D4();
		    break;
		  case 5:
		    test2DN1();
		    break;
		  case 6:
		    test2DN2();
		    break;
		  case 7:
		    test2DN3();
		    break;
		  case 8:
		    test3D();
		    break;
		  case 9:
		    test3D1();
		    break;
		  case 10:
		    test3D2();
		    break;
		  case 11:
		    testArrows();
		    break;
		  case 12:
		    testC1();
		    break;
		  case 13:
		    testC2();
		    break;
		  case 14:
		    testC3();
		    break;
		  case 15:
		    testCh();
		    break;
		  case 16:
		    testG();
		    break;
		  case 17:
		    testP3D();
		    break;
		  case 18:
		    testPattern();
		    break;
		  case 19:
		    testColor();
		    break;
		  case 20:
		    testPrim();
		    break;
		  case 21:
		    testString();
		    break;
		  case 22:
		    testXliness();
		    break;
		  case 23:
		    testXrects();
		    break;
		  }

		break;
	  }
}
/* end HandleMenu */
