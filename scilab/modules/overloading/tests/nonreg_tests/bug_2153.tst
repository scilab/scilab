//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2153 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2153
//
// <-- Short Description -->
//    addition and subtraction of matrix and hypermatrix of same dimension and size do not work

if execstr("zeros(1,3) + matrix([0 0 0], [1 3])", "errcatch")<>0 then pause,end
