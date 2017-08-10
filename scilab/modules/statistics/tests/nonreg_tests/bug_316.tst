//<-- CLI SHELL MODE -->
// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- Non-regression test for bug 316 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=316
//
// <-- Short Description -->
//      The description for the "Mean" function is in error:
// the parameter "r" really does the column average, while the "c"
// parameter does the row

a = [ 1 1 1 ; 2 2 2 ; 3 3 3 ];

if size(mean(a,'r')) <> [ 1, 3] then pause, end
if size(mean(a,'c')) <> [ 3, 1] then pause, end
