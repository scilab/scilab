//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3121 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3121
//
// <-- Short Description -->
//The error message returned by zeros() when input arguments are not integers is not clear :
if or(zeros(2.1,2.1,2.1)<>zeros(2,2,2)) then pause,end
if or(ones(2.1,2.1,2.1)<>ones(2,2,2)) then pause,end
if or(size(rand(2.1,2.1,2.1))<>[2 2 2]) then pause,end
