// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 6861 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6861
//
// <-- Short Description -->
// yulewalk example failed on Windows

f = [0, 0.4, 0.4, 0.6, 0.6, 1];
H = [0, 0, 1, 1, 0, 0];
ierr = execstr("Hz = yulewalk(8, f, H);","errcatch");
if ierr <> 0 then pause,end
