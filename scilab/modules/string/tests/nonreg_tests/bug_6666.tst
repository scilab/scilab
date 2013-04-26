// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 6666 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6666
//
// <-- Short Description -->
// emptystr() gave a wrong result instead of complaining about hypermatrix.
//========================================================================================
M = rand(7, 3);
T = emptystr(M);
if or(size(T) <> [ 7 3 ]) then pause, end

M = rand(2, 2, 2);
ierr = execstr("T = emptystr(M);","errcatch");
if ierr <> 0 then pause, end
//========================================================================================
