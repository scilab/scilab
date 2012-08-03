// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8413 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8413
//
// <-- Short Description -->
// fscanfMat crashed when file included text lines at the beginning.
//

ierr = execstr("[r,v] = fscanfMat(""SCI/modules/fileio/tests/nonreg_tests/bug_8413.dat"");", "errcatch");
if ierr <> 0 then pause, end
if type(v) <> 10 then pause, end
if type(r) <> 1 then pause, end
if size(v, "*") <> 1 then pause, end
if size(r, "r") <> 3 then pause, end
if size(r, "c") <> 8 then pause, end
