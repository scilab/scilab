//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3627 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3627
//
// <-- Short Description -->
// Variance for complex data


N=1000;
x = grand(N,1,'nor',0,1) + %i*grand(N,1,'nor',0,1);
computed = mtlb_var(x);
expected = 2.0;
if abs(computed-expected)>1. then pause,end

