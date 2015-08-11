// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13976 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13976
//
// <-- Short Description -->
// grand("setsd",S) failed though S has a right values.

S=grand("getsd");
[a(1) a(2)] = size(S);
assert_checkequal(a, [625;1]);
// must not produce any error.
grand("setsd",S);
