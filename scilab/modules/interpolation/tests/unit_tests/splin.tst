//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


function y=runge(x)
    y=1 ./(1 + x.^2)
endfunction

a = -5; b = 5; n = 11; m = 400;
x = linspace(a, b, n)';
y = runge(x);
d = splin(x, y);
xx = linspace(a, b, m)';
yyi = interp(xx, x, y, d);
yye = runge(xx);

assert_checkequal(size(d), [11 1]);
assert_checkequal(d(6), 0);
assert_checkfalse(d(7:11) > 0);

// interval of interpolation
a = 0;
b = 1;
// nb of interpolation  points
n = 10;
// discretization for evaluation
m = 800;
// abscissae of interpolation points
x = linspace(a,b,n)';
// ordinates of interpolation points
y = rand(x);

xx = linspace(a,b,m)';

yk = interp(xx, x, y, splin(x,y,"not_a_knot"));
assert_checkequal(size(yk), [800 1]);

yf = interp(xx, x, y, splin(x,y,"fast"));
assert_checkequal(size(yf), [800 1]);

ym = interp(xx, x, y, splin(x,y,"monotone"));
assert_checkequal(size(yf), [800 1]);
