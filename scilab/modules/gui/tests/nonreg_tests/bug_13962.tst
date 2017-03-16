// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13962 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13962
//
// <-- Short Description -->
// If the groupname and value properties were setted to radiobutton creation, 
// then it was only selected at the first execution.

function test()
f = createWindow();
f.figure_name = "test";
f.axes_size = [200 200];
f.tag = "test";
f.background = -2;

r1 = uicontrol(f, "style", "radiobutton", ... 
"string", "radiobutton1", ...
"groupname", "testgroup", ...
"value", 1, ...
"tag", "r1", ...
"position", [0 90 200 20]);
endfunction

test();
// check that the radiobutton is well selected.
close();

test();
// check that the radiobutton is well selected.
close();
