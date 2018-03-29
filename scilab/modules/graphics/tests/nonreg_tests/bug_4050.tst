// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4050 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4050
//
// <-- Short Description -->
// With ged(), editing a graphical text having a font size > 5 set the font
// size to 5.

plot(1:10)
xstring(2,7,"Texte")
gce().font_size = 7;
ged(9);
// Go to the Text object
// See the slider for its font size
// * Check that it is set to 7.
// * Check that the max tunable size is 12
// * Click on the right to increase the size, up to 9
// * Quit the editor
gce().font_size   // should display 9
ged(9);
// Go to the Text object, and to its font size slider
// * Check that it is set to 9
// * Check that the maximal tunable font size is 14
// * Click on the left to decrease the font size, down to 1
// * Quit the editor
gce().font_size   // should display 1
ged(9);
// Go to the Text object, and to its font size slider
// * Check that it is set to 1
// * Check that the maximal tunable font size is 5
