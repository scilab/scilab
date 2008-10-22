// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a = 0; b = 2*%pi;
sigma = 0.1;  // standard deviation of the gaussian noise
m = 200;       // number of experimental points
xd = linspace(a,b,m)';
yd = sin(xd) + grand(xd,"nor",0,sigma);

n = 6; // number of breakpoints
x = linspace(a,b,n)';

// compute the spline
[y, d] = lsq_splin(xd, yd, x);  // use equal weights
if or(size(y)<> [6 1]) then pause,end
if or(size(d)<> [6 1]) then pause,end
