// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14637 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14637
//
// <-- Short Description -->
// Some Scilab 5.5.2 diagrams didn't simulate properly in Xcos 6

assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/nonreg_tests/bug_14637_1.zcos"));
xcos_simulate(scs_m, 4);

clear scs_m;

assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/nonreg_tests/bug_14637_2.zcos"));
xcos_simulate(scs_m, 4);
