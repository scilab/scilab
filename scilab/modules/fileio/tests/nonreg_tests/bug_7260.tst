// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7260 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7260
//
// <-- Short Description -->
// read a malformed .csv file with fscanfMat crashed Scilab.
//
ierr = execstr("fscanfMat(""SCI/modules/fileio/tests/nonreg_tests/bug_7260_longline_NOK.csv"")", "errcatch");
if ierr <> 999 then pause, end

M = fscanfMat("SCI/modules/fileio/tests/nonreg_tests/bug_7260_longline_OK.csv");
s = size(M);
if s(1) <> 1 then pause,end
if s(2) <> 8 then pause,end

ierr = execstr("fscanfMat(""SCI/modules/fileio/tests/nonreg_tests/bug_7260_shortline_NOK.csv"")", "errcatch");
if ierr <> 999 then pause, end

M = fscanfMat("SCI/modules/fileio/tests/nonreg_tests/bug_7260_shortline_OK.csv");
s = size(M);
if s(1) <> 1 then pause,end
if s(2) <> 7 then pause,end
