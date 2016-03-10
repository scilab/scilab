// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// Unit test for imag
assert_checkequal(imag([]),[]);
// with scalar real numbers
assert_checkequal(imag(%pi),0);
assert_checkequal(imag(%inf),0);
assert_checkequal(imag(-%inf),0);
assert_checkequal(imag(%nan),0);
assert_checkequal(imag(-%nan),0);
// with scalar complex numbers
c = complex(1, 1);       assert_checkequal(imag(c),1);
c = complex(1, 0);       assert_checkequal(imag(c),0);
c = complex(1, -10);     assert_checkequal(imag(c),-10);
c = complex(1, %inf);    assert_checkequal(imag(c),%inf);
c = complex(-%inf, %inf); assert_checkequal(imag(c),%inf);
c = complex(0, -%inf);   assert_checkequal(imag(c),-%inf);
c = complex(%nan, %pi);  assert_checkequal(imag(c),%pi);
c = complex(%nan, %nan); assert_checkequal(imag(c),%nan);
// With hypermatrix of complex numbers
r = rand(3,4,2);
c = complex(-r,r); assert_checkequal(imag(c),r);
r(2,2,2) = %nan;
r(3,1,1) = %inf;
r(2,4,2) = -%inf;
c = complex(-r, r); assert_checkequal(imag(c),r);
// With a sparse complex matrix
r = sprand(100,100,0.001);
i = sqrt(r);
s = r+i*%i;
assert_checkequal(imag(s),i);
// With a scalar polynomial
r = rand(1, 10) - 0.5;
p = poly(r, "x", "coeff");
assert_checkequal(imag(p), 0*%z);
pc = poly(complex(r,-r), "x", "coeff");
assert_checkequal(imag(pc), -p);
// With a matrix of real and complex polynomials
pc = [p, pc ; -p-%i, -pc+%i];
assert_checkequal(imag(pc),[0, -p; -1, 1+p]);
// With a matrix of rationals
r = [(1-%z)^2 / %z - %i , (%z + %i)/(1+%z)];
assert_checkequal(imag(r),[-1/1+0*%z 1/(1+%z)]);
