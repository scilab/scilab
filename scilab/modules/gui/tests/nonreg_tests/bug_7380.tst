// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7380 -->
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7380
//
// <-- Short Description -->
// Any input to u.value is rounded for uicontrols.
// It is not possible to continuously tune some real value such as an angle.

u = uicontrol("style", "slider", ..
              "position", [10 10 200,30], ..
              "max", %pi);

u.value = 0.6;

assert_checkequal(u.value, 0.6);
