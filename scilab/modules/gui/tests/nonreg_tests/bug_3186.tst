// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 3186 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3186
//
// <-- Short Description -->
// input is lost when graphics window is closed


// Try: plot2d();ans=input("Save (y or n)?","string")

// First close the graphics window, and second answer the question in the shell by entering y and then <enter>.

// Check that: ans=="y" and you do not get an error such as "undefined variable: y"