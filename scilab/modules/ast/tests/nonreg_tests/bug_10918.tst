// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 10918 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10918
//
// <-- Short Description -->
// After a=5; a([%t %f]') returned [5 %eps]' instead of 5.

a = 5;
assert_checkequal(a([%t %f]'), 5);
assert_checkequal(a([%t %f]), 5);
