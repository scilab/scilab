// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 5291 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5291
//
// <-- Short Description -->
// [R,F]=strtod(S) should return %nan instead of 0 as interpreted real number when
// no real number can be detected at the beginning of processed strings.
//

[R, F]=strtod([ "123" "+i"]);
assert_checkequal(R(1), 123);
assert_checktrue(isnan(R(2)));
assert_checkequal(F, ['' '+i']);
