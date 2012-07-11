// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 2395 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2395
//
// <-- Short Description -->
//   mfile2sci() does not complete when converting the following function.
//   undefined variable : mfile_path

ierr = execstr("mfile2sci(""SCI/modules/m2sci/tests/nonreg_tests/bug_2395.m"", TMPDIR, %T, %T, 3, %T);", "errcatch");
if ierr<>0 then pause;end
