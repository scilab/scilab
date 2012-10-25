/ =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 5267 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5267
//
// <-- Short Description -->
// Xcos diagram causes Scilab to quit if Simulation/Setup is selected, then cancelled using the "x" on the upper right.


// Start xcos

// Simulation >> Setup

// Close the setup window using the upper right cross

// Check that Scilab does not exit
