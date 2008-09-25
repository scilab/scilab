// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


deff("y=runge(x)","y=1 ./(1 + x.^2)")
a = -5; b = 5; n = 11; m = 400;
x = linspace(a, b, n)';
y = runge(x);
d = splin(x, y);
xx = linspace(a, b, m)';
yyi = interp(xx, x, y, d);
yye = runge(xx);

if or(size(d) <> [11 1]) then pause,end
if d(6) <> 0 then pause,end
if d(7:11) > 0 then pause,end

a = 0; b = 1;        // interval of interpolation
n = 10;              // nb of interpolation  points
m = 800;             // discretisation for evaluation
x = linspace(a,b,n)'; // abscissae of interpolation points
y = rand(x);          // ordinates of interpolation points
xx = linspace(a,b,m)';

yk = interp(xx, x, y, splin(x,y,"not_a_knot"));
if or(size(yk) <> [800 1]) then pause,end

yf = interp(xx, x, y, splin(x,y,"fast"));
if or(size(yf) <> [800 1]) then pause,end

ym = interp(xx, x, y, splin(x,y,"monotone"));
if or(size(yf) <> [800 1]) then pause,end
 