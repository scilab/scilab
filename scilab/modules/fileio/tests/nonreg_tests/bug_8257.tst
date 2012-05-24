// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8257 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8257
//
// <-- Short Description -->
// fscanfMat did not read matrix file not formated by fprintfMat.
//

ierr = execstr("r = fscanfMat(""SCI/modules/fileio/tests/nonreg_tests/bug_8257.txt"");", "errcatch");
if ierr <> 0 then pause, end
ref = [ 1.     2.     3.     4.     5.;
       3.2    5.2    6.6    0.2    5.9];
if size(r, "c") <> size(ref, "c") then pause, end
if size(r, "r") <> size(ref, "r") then pause, end
if or(r <> ref) then pause, end
