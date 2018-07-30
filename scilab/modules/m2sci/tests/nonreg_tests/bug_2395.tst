// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 2395 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/2395
//
// <-- Short Description -->
//   mfile2sci() does not complete when converting the following function.
//   undefined variable : mfile_path

ierr = execstr("mfile2sci(""SCI/modules/m2sci/tests/nonreg_tests/bug_2395.m"", TMPDIR, %T, %T, 3, %T);", "errcatch")
assert_checkequal(ierr, 0);
