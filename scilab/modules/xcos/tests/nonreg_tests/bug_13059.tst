// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 13059 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13059
//
// <-- Short Description -->
// '%nan' propagated at startup fail the simulation

loadXcosLibs();

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_13059.zcos"));

// check using scicos_simulate
scicos_simulate(scs_m);

assert_checkequal(A.values, %nan);
clear A;

// check using xcos_simulate
xcos_simulate(scs_m, 4);

assert_checkequal(A.values, %nan);
clear A;

