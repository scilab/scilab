// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 7315 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7315
//
// <-- Short Description -->
// The Affich block doesn't refresh values.

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_7315.zcos");
// start the simulation
// Value must be updated

