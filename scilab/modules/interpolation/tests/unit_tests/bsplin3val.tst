//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

function v=f(x,y,z)
    v=cos(x).*sin(y).*cos(z);
endfunction

function v=fx(x,y,z)
    v=-sin(x).*sin(y).*cos(z)
endfunction

function v=fxy(x,y,z)
    v=-sin(x).*cos(y).*cos(z)
endfunction

function v=fxyz(x,y,z)
    v=sin(x).*cos(y).*sin(z)
endfunction

function v=fxxyz(x,y,z)
    v=cos(x).*cos(y).*sin(z)
endfunction

// n x n x n  interpolation points
n = 20;
x = linspace(0,2*%pi,n);

// interpolation grid
y=x; z=x;
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);

// compute f and some derivates on a point
// and compare with the spline interpolant
xp = grand(1,1,"unf",0,2*%pi);
yp = grand(1,1,"unf",0,2*%pi);
zp = grand(1,1,"unf",0,2*%pi);

f_e = f(xp,yp,zp);
f_i = bsplin3val(xp,yp,zp,tl,[0 0 0]);
assert_checkfalse(abs(f_i) > 1);

fx_e = fx(xp,yp,zp);
fx_i = bsplin3val(xp,yp,zp,tl,[1 0 0]);
assert_checkfalse(abs(fx_i) > 1);

fxy_e = fxy(xp,yp,zp);
fxy_i = bsplin3val(xp,yp,zp,tl,[1 1 0]);
assert_checkfalse(abs(fxy_i) > 1);

fxyz_e = fxyz(xp,yp,zp);
fxyz_i = bsplin3val(xp,yp,zp,tl,[1 1 1]);
assert_checkfalse(abs(fxyz_i) > 1);

fxxyz_e = fxxyz(xp,yp,zp);
fxxyz_i = bsplin3val(xp,yp,zp,tl,[2 1 1]);
assert_checkfalse(abs(fxxyz_i) > 1);
