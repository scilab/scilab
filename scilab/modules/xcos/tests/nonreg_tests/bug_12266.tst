// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Alexandre HERISSE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 12266 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12266
//
// <-- Short Description -->
// Stopping "Papillon de lorenz" demonstration while simulation was on made CMSCOPE crash. 

clear is_crashed;
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_12266.zcos"));
xcos_simulate(scs_m, 4);
assert_checkequal(length(is_crashed.values), 6);

clear is_crashed;
// if segfault appeared on cmscope block at the simulation end, then the second xcos_simulate will fail
xcos_simulate(scs_m, 4);
assert_checkequal(length(is_crashed.values), 6);

