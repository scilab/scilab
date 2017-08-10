// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// Unit test for real
assert_checkequal(real([]),[]);
// with scalar real numbers
assert_checkequal(real(%pi),%pi);
assert_checkequal(real(%inf),%inf);
assert_checkequal(real(-%inf),-%inf);
assert_checkequal(real(%nan),%nan);
assert_checkequal(real(-%nan),%nan);
// with scalar complex numbers
c = complex(0, 1);       assert_checkequal(real(c),0);
c = complex(1, 1);       assert_checkequal(real(c),1);
c = complex(-10, -1);    assert_checkequal(real(c),-10);
c = complex(%inf, 1);    assert_checkequal(real(c),%inf);
c = complex(%inf, -%inf); assert_checkequal(real(c),%inf);
c = complex(-%inf, 0);   assert_checkequal(real(c),-%inf);
c = complex(%pi, %nan);  assert_checkequal(real(c),%pi);
c = complex(%nan, %nan); assert_checkequal(real(c),%nan);
// With hypermatrix of complex numbers
r = rand(3,4,2);
c = complex(-r,r); assert_checkequal(real(c), -r);
r(2,2,2) = %nan;
r(3,1,1) = %inf;
r(2,4,2) = -%inf;
c = complex(-r, r); assert_checkequal(real(c), -r);
// With a sparse complex matrix
r = sprand(100,100,0.001);
i = sqrt(r);
s = r+i*%i;
assert_checkequal(real(s), r);
// With a scalar polynomial
r = rand(1, 10) - 0.5;
p = poly(r, "x", "coeff");
assert_checkequal(real(p), p);
pc = poly(complex(r,-r), "x", "coeff");
assert_checkequal(real(pc), p);
// With a matrix of real and complex polynomials
pc = [p, pc ; -p-%i, -pc+%i];
assert_checkequal(real(pc),[p, p; -p, -p]);
// With a matrix of rationals
r = [(1-%z)^2 / %z - %i , (%z + %i)/(1+%z)];
assert_checkequal(real(r),[(1-%z)^2 / %z , %z/(1+%z)]);
