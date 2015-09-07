//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3827 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3827
//
// <-- Short Description -->
//   Atlas library crashs scilab with this example

a=[1 2 3;2 3 4;3 4 5];
ev=spec(a);
if or(size(ev)<> [3 1]) then pause,end
