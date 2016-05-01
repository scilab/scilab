//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a = 0; b = 2*%pi;
// standard deviation of the gaussian noise
sigma = 0.1;
// number of experimental points
m = 200;
xd = linspace(a,b,m)';
yd = sin(xd) + grand(xd,"nor",0,sigma);

// number of breakpoints
n = 6;
x = linspace(a,b,n)';

// compute the spline

// use equal weights
[y, d] = lsq_splin(xd, yd, x);
assert_checkequal(size(y), [6 1]);
assert_checkequal(size(d), [6 1]);
