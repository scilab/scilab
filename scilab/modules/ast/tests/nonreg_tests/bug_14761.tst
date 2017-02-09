// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14761 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14761
//
// <-- Short Description -->
//
// || misworks when LHS is %f or zeros. && misworks when LHS is %t or non-zeros

m = [4 9 8;1 5 3];
z = zeros(2,3);

assert_checkequal(m || %f, and(m));
assert_checkequal(m || %t, %t);
assert_checkequal(%f || m, and(m));
assert_checkequal(%t || m, %t);

assert_checkequal(z || %f, and(z));
assert_checkequal(z || %t, %t);
assert_checkequal(%f || z, and(z));
assert_checkequal(%t || z, %t);

assert_checkequal(m && %f, %f);
assert_checkequal(m && %t, and(m));
assert_checkequal(%f && m, %f);
assert_checkequal(%t && m, and(m));

assert_checkequal(z && %f, %f);
assert_checkequal(z && %t, and(z));
assert_checkequal(%f && z, %f);
assert_checkequal(%t && z, and(z));

//introduce a 0
m(2,2)=0;

assert_checkequal(m || %f, and(m));
assert_checkequal(m || %t, %t);
assert_checkequal(%f || m, and(m));
assert_checkequal(%t || m, %t);

assert_checkequal(z || %f, and(z));
assert_checkequal(z || %t, %t);
assert_checkequal(%f || z, and(z));
assert_checkequal(%t || z, %t);

assert_checkequal(m && %f, %f);
assert_checkequal(m && %t, and(m));
assert_checkequal(%f && m, %f);
assert_checkequal(%t && m, and(m));

assert_checkequal(z && %f, %f);
assert_checkequal(z && %t, and(z));
assert_checkequal(%f && z, %f);
assert_checkequal(%t && z, and(z));
