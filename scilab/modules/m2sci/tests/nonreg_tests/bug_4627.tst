// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4627 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4627
//
// <-- Short Description -->
//    The function matfile2sci does not manage 7.x format.

ierr = execstr("matfile2sci(""SCI/modules/m2sci/tests/nonreg_tests/bug_4627.mat"", ""TMPDIR/bug_4627.bin"");", "errcatch");
if ierr<>0 then pause;end
ierr = execstr("load(""TMPDIR/bug_4627.bin"");", "errcatch");
if ierr<>0 then pause;end
if a<>10 | b<>20 then pause;end