// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15468 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15468
//
// <-- Short Description -->
// Scilab 5.5.2 models loaded and saved back using Scilab 6 might contain non-utf8 characters.
//

scs_m = xcosDiagramToScilab(SCI+"/modules/xcos/tests/nonreg_tests/bug_15468.zcos");
xcosDiagramToScilab(TMPDIR+"/bug_15468.zcos", scs_m);
scs_m = xcosDiagramToScilab(TMPDIR+"/bug_15468.zcos");

