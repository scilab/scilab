// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4075 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4075
//
// <-- Short Description -->
// a basic uicontrol example crashed scilab 5.0.3

clear;
clc;
fig = figure();
B1 = uicontrol(fig, "style", "pushbutton", "string", "1", ..
"position", [50 50 50 20], "callback", "disp(1)");
exit_condition = %f;
while ~exit_condition
    sleep(10)
end

