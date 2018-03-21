// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Paul Bignier
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15495 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15495
//
// <-- Short Description -->
// SampleCLK simple demo failed to simulate.
//

scs_m = xcosDiagramToScilab(SCI+"/modules/xcos/tests/nonreg_tests/bug_15495.zcos");
xcos_simulate(scs_m, 4);
