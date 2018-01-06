// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 15359 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15359
//
// <-- Short Description -->
// twinkle() was unable to blink several independent objects

clf
plot()
c = gca().children.children(1:10);
c(1:5).visible = "off";
twinkle(c)      // A whole subset of 10 curves of the bottom axes must blink 5 times
twinkle(c,3)    // A whole subset of 10 curves of the bottom axes must blink 3 times
// In both cases, in the final state, 5 curves must be invisible as before blinking.
