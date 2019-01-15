// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 4953 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4953
//
// <-- Short Description -->
// The clean function does not work when the matrix contains the %inf value.

// with %inf
big = %inf
small = 1;
a1 = [big small];
a2 = [big complex(small,small)];
a3 = [complex(0,big) small complex(big,small)];
//double
assert_checkequal(clean(a1),a1);
assert_checkequal(clean(a2),a2);
assert_checkequal(clean(a3),a3);
//polynomial
assert_checkequal(clean(poly(a1,"x","coeff")),poly(a1,"x","coeff"));
assert_checkequal(clean(poly(a2,"x","coeff")),poly(a2,"x","coeff"));
assert_checkequal(clean(poly(a3,"x","coeff")),poly(a3,"x","coeff"));
//sparse
assert_checkequal(clean(sparse(a1)),sparse(a1));
assert_checkequal(clean(sparse(a2)),sparse(a2));
assert_checkequal(clean(sparse(a3)),sparse(a3));

// with %nan
big = 1e30
small = 1;
a4 = [small big %nan];
b4 = [0 big %nan];
//double
c4 = clean(a4);
assert_checktrue(c4(1:2) == b4(1:2) && isnan(c4(3)))
//poly
c4 = clean(poly(a4,"x","coeff"));
assert_checktrue(coeff(c4)(1:2) == b4(1:2) && isnan(coeff(c4)(3)))
//sparse
c4 = clean(sparse(a4));
assert_checktrue(c4(1:2) == b4(1:2) && isnan(c4(3)))

