// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
//
// <-- Non-regression test for bug 9357 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9357
//
// <-- Short Description -->
//stdevf returns zero for row vector arguments

a=[2,3,4,5,5];
b=[2,3,4,5];
fre=[1,1,1,2];
ref= stdev(a);
assert_checkequal(stdevf(b,fre),ref);
assert_checkequal(stdevf(b,fre,'*'),ref);
assert_checkequal(stdevf(b,fre,2),ref);
assert_checkequal(stdevf(b,fre,1),zeros(b));
