// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 12620 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12620
//
// <-- Short Description -->
// Size of uicontextmenu is not correct when we want to display all uimenu
// (which are in uicontextmenu) in the same time, in other words, when using
// "visible" property.

plot2d();
[ibutton,xcoord,yxcoord,iwin,cbmenu]=xclick();
if ibutton== 5 then //Right click on graph
    hMenu = uicontextmenu();
    hItem1 = uimenu("Label", "AAAAAAAAAAAAAAAA", "Parent", hMenu,"visible","off");
    hItem2 = uimenu("Label", "BBBBBBBBBBBBBBBB", "Parent", hMenu,"visible","off");
    hItem3 = uimenu("Label", "CCCCCCCCCCCCCCCC", "Parent", hMenu,"visible","off");
    hItem4 = uimenu("Label", "DDDDDDDDDDDDDDDD", "Parent", hMenu,"visible","off");
    hItem5 = uimenu("Label", "EEEEEEEEEEEEEEEE", "Parent", hMenu,"visible","off");
    hItem6 = uimenu("Label", "FFFFFFFFFFFFFFFF", "Parent", hMenu,"visible","off");
    set([hItem1;hItem2;hItem3;hItem4;hItem5;hItem6],"visible","on");
end

// Riht-clic in the figure

// Check the menus in the context menu are OK.
