// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 13549 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13549
//
// <-- Short Description -->
// Slider uicontrol triggers 3 callbacks instead of one

h=uicontrol("Style", "slider", ...
"Units","normalized", ...
"Position", [0.125 0.1 .75 .25], ...
"Callback","disp(gcbo.value)");

// Click on the slider
// Check that the value appears only once in the console