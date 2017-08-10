//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11954 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11954
//
// <-- Short Description -->
//
// save with two times the same name leads to an HDF5 error.

x=123;
assert_checktrue(execstr("save(TMPDIR + ""/toto"" ,""x"", ""x"")", "errcatch") == 0);
assert_checktrue(execstr("save(TMPDIR + ""/bug_11954.sod"", ""x"", ""x"", ""-append"")", "errcatch") == 0);
