//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - S/E - Sylvestre Ledru
// Copyright (C) 2013 - Alex Carneiro
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
s=poly(0,'s');
assert_checkequal(derivat(1/s), -1/s^2);

p1 = poly([1 -2 1], 'x', 'coeff');
x=poly(0,'x');
assert_checkequal(derivat(p1),-2 + 2*x);

p2 = poly([1 -4 2], 'x', 'coeff');
assert_checkequal(derivat(p2),-4 + 4*x);

p3 = poly(ones(1, 10), 'x', 'coeff');
assert_checkequal(derivat(p3), 1+2*x+3*x^2+4*x^3+5*x^4+6*x^5+7*x^6+8*x^7+9*x^8);

p4 = poly([-1 1], 'x', 'roots');
assert_checkequal(derivat(p4), 2*x);

s = %s; p5 = s^-1 + 2 + 3*s;
assert_checkequal(derivat(p5),(-1+3*s^2)/s^2);
