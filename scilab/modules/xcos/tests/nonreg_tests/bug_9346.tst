// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 9346 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9346
//
// <-- Short Description -->
// FROMWS block does not works


status = importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_9346.zcos");
if ~status then pause, end;

V.time = (1:10)';
V.values = matrix(1:50, [10 5]);
xcos_simulate(scs_m, 4);

