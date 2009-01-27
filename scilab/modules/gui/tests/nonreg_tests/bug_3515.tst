// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 3515 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3515
//
// <-- Short Description -->
// Erroneous submenu numbering for setmenu() and unsetmenu() in Scilab 5.0.1
// Submenus seems shifted by 1 in Scilab 5.0.1, besides doing an "out of bounds" setmenu causes a stack problem, see below.

addmenu("Hello",["Franck";"Peter"])
unsetmenu("Hello", -1); // Bad index 
unsetmenu("Hello", 0); // Bad index 
unsetmenu("Hello", 1); // Franck
unsetmenu("Hello", 2); // Peter
unsetmenu("Hello", 3); // Bad index

setmenu("Hello", -1); // Bad index 
setmenu("Hello", 0); // Bad index 
setmenu("Hello", 1); // Franck
setmenu("Hello", 2); // Peter
setmenu("Hello", 3); // Bad index 