// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 9307 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9307
//
// <-- Short Description -->
// AFFICH_m block only printed the first column for an input matrix.

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_9307.zcos");
// Simulate
// Check that the AFFICH_m block value is :
//      1.000     2.000     3.000
//      4.000     5.000     6.000
//      7.000     8.000     9.000



