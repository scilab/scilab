// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 7015 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7015
//
// <-- Short Description -->
// The palette must be always visible when an empty diagram is created. When 
// the diagram is loaded from a file, the palette may be hidden.

// xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_6386.zcos");
// Check that the palette is not visible
// Close Xcos
// xcos()
// Check that the palette is set visible
// Close the palette
// xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_6386.zcos");
// Check that the palette is set visible.



