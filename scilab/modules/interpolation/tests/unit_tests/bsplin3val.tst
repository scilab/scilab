// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

deff("v=f(x,y,z)","v=cos(x).*sin(y).*cos(z)");
deff("v=fx(x,y,z)","v=-sin(x).*sin(y).*cos(z)");
deff("v=fxy(x,y,z)","v=-sin(x).*cos(y).*cos(z)");
deff("v=fxyz(x,y,z)","v=sin(x).*cos(y).*sin(z)");
deff("v=fxxyz(x,y,z)","v=cos(x).*cos(y).*sin(z)");
n = 20;  // n x n x n  interpolation points
x = linspace(0,2*%pi,n); y=x; z=x; // interpolation grid
[X,Y,Z] = ndgrid(x,y,z); V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);

// compute f and some derivates on a point
// and compare with the spline interpolant 
xp = grand(1,1,"unf",0,2*%pi); 
yp = grand(1,1,"unf",0,2*%pi); 
zp = grand(1,1,"unf",0,2*%pi); 

f_e = f(xp,yp,zp);
f_i = bsplin3val(xp,yp,zp,tl,[0 0 0]);
if abs(f_i) > 1 then pause,end

fx_e = fx(xp,yp,zp);
fx_i = bsplin3val(xp,yp,zp,tl,[1 0 0]);
if abs(fx_i) > 1 then pause,end

fxy_e = fxy(xp,yp,zp);
fxy_i = bsplin3val(xp,yp,zp,tl,[1 1 0]);
if abs(fxy_i) > 1 then pause,end

fxyz_e = fxyz(xp,yp,zp);
fxyz_i = bsplin3val(xp,yp,zp,tl,[1 1 1]);
if abs(fxyz_i) > 1 then pause,end

fxxyz_e = fxxyz(xp,yp,zp);
fxxyz_i = bsplin3val(xp,yp,zp,tl,[2 1 1]);
if abs(fxxyz_i) > 1 then pause,end
