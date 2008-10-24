// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 3554 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3554
//
// <-- Short Description -->
//    The function 'findobj' can not find figures
//    The function 'findobj' can not find submenus

// Try to find a figure
f=figure("tag","test figure");
if isempty(findobj("tag","test figure")) then pause; end
close(f);

// Try to find a menu
f=figure("tag","test figure");
m=uimenu(f,"tag","test menu","label","test");
if isempty(findobj("tag","test menu")) then pause; end
close(f);

// Try to find a submenu
f=figure("tag","test figure");
m=uimenu(f,"tag","test menu","label","test");
m1=uimenu(m,"tag","test submenu","label","testsub");
if isempty(findobj("tag","test submenu")) then pause; end
close(f);

// Try to find an uicontrol
f=figure("tag","test figure");
h=uicontrol(f,"style","frame","tag","test frame");
if isempty(findobj("tag","test frame")) then pause; end
close(f);

// Try to find an uicontrol when it is a frame child
f=figure("tag","test figure");
h=uicontrol(f,"style","frame","tag","test frame");
h1=uicontrol(h,"style","pushbutton","tag","test pushbutton");
if isempty(findobj("tag","test pushbutton")) then pause; end
close(f);



                                       