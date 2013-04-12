// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8630 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8630
//
// <-- Short Description -->
// Scilab crash when simulating scopes with the same window ids.


status = importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_8630.zcos");
if ~status then pause, end

prot = funcprot();
funcprot(0);
deff("disp(str)", "");
funcprot(prot);

// compile and simulate
xcos_simulate(scs_m, 4);

clear disp;

