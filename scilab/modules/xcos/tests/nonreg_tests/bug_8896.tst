// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8896 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8896
//
// <-- Short Description -->
// On windows, a multi-line context could not be edited twice.
//

xcos
// Simulation -> context
// Type:
//     A = 43;
//     B = A - 1;
// OK
// Simulation -> context
// OK should not produce an error on the scilab console


