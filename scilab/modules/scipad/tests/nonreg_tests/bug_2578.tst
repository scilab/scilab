// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2578 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2578
//
// <-- Short Description -->
//    The input function does not seem to work properly. In particular if the script is launched from scipad.

// 1 - Start Scilab

// 2 - Start Scipad

// 3 - In Scipad enter:
// x=input('enter number')
// disp(x)

// 4 - "Load into Scilab"

// 5 - Enter a value in Scilab console

// 6 - Check that x is not empty
