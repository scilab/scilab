// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11685 -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11685
//
// <-- Short Description -->
// gcbo variable was not defined when executing functions set
// as "closerequestfcn" and "resizefcn" properties.

function resizeMe()
  disp(gcbo);
endfunction
function closeMe()
  disp(gcbo);
  delete(gcbo)
endfunction

f = figure();
f.resizefcn = "resizeMe";
f.closerequestfcn = "closeMe";

// Resize the figure & check that gcbo is displayed in the console

// Click on the top-right cross of the figure to close it
// - check that gcbo is displayed in the console
// - check that the figure is closed


