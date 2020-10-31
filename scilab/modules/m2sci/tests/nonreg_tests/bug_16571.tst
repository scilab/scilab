// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 16571 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16571
//
// <-- Short Description -->
// 1) ~true(2,3) was converted into ~ones(2,3)==1 instead of ~(ones(2,3)==1)
// 2) ~ applied to integer contents yielded an error from convert2double()
// 3) ~ applied to complex contents forgot applying abs() before ~


mfile = SCI+"/modules/m2sci/tests/nonreg_tests/bug_16571.m";
mfile2sci(mfile, TMPDIR);
mprintf("%s\n", mgetl(TMPDIR + "/bug_16571.sci"));
