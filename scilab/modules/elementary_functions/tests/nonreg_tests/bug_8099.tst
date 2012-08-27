// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8099 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8099
//
// <-- Short Description -->
// the cumprod function did not simplify the result when it is complex.

c = cumprod([1/%z %z/(%i+2*%z)]);
res = [1/%z, 1/(%i+2*%z)];
assert_checkequal(c(2), res(2));
assert_checkequal(c(3), res(3));
