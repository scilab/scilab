// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8837 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8837
//
// <-- Short Description -->
//
// fscanfMat did not read a malformated text with many blanks at the end of file.

if execstr("r = fscanfMat(SCI + ""/modules/fileio/tests/nonreg_tests/bug_8837.txt"")", "errcatch") <> 0 then pause, end
if size(r, "r") <> 9 then pause, end
if size(r, "c") <> 7 then pause, end
if r(1, 1) <> 4 then pause, end
if r(9, 7) <> -81.38 then pause, end