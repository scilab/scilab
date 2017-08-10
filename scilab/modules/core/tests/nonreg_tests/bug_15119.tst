// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 15119 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15119
//
// <-- Short Description -->
// Inequality comparisons between complex-encoded real numbers failed

r0 = -1 + 0*%i;
r = r0;
assert_checktrue(r<0);
assert_checktrue(r<=0);
assert_checkfalse(r>=0);
assert_checkfalse(r>0);

r = ones(2,3) * r0;
assert_checktrue(r<0);
assert_checkequal(size(r<0), [2 3]);
assert_checktrue(r<=0);
assert_checkequal(size(r<=0), [2 3]);
assert_checkfalse(r>=0);
assert_checkequal(size(r>=0), [2 3]);
assert_checkfalse(r>0);
assert_checkequal(size(r>0), [2 3]);

r = ones(2,3,2) * r0;
assert_checktrue(r<0);
assert_checkequal(size(r<0), [2 3 2]);
assert_checktrue(r<=0);
assert_checkequal(size(r<=0), [2 3 2]);
assert_checkfalse(r>=0);
assert_checkequal(size(r>=0), [2 3 2]);
assert_checkfalse(r>0);
assert_checkequal(size(r>0), [2 3 2]);
