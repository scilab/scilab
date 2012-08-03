// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7432 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7432
//
// <-- Short Description -->
// fprintfMat with a non valid format as '%s' can create a wrong file
//
//==============================================================================
ierr = execstr("fprintfMat(TMPDIR + ""/test.txt"", ones(3,3), ""%s"")", "errcatch");
if ierr <> 999 then pause, end
//==============================================================================
fprintfMat(TMPDIR + "/test.txt", ones(3,3), "%d");
ierr = execstr("fscanfMat(TMPDIR + ""/test.txt"", ""%s"")", "errcatch");
if ierr <> 999 then pause, end
//==============================================================================

