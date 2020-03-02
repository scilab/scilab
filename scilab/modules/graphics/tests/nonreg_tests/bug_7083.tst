// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7083 -->
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7083
//
// <-- Short Description -->
// Setting uicontrol.backgroundcolor=[10 10 10] with RGB values > 1
// yielded a critical error and a stack problem.


h = gcf();
h_tb = uicontrol(h,'style','text', 'position', [10 10 20 200]);
set(h_tb,'backgroundcolor',[1 1 1]);

msg = "Wrong value for ''BackgroundColor'' property: Numbers between 0 and 1 expected.";
assert_checkerror("set(h_tb,''backgroundcolor'',[10 10 10]);", msg);
assert_checkequal(h_tb.backgroundcolor, [1 1 1]);
assert_checkerror("h_tb; h_tb.backgroundcolor=[10 10 10];", msg);
assert_checkequal(h_tb.backgroundcolor, [1 1 1]);
