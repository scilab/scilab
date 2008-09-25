// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

func =  "v=cos(2*%pi*x).*sin(2*%pi*y).*cos(2*%pi*z)";
deff("v=f(x,y,z)",func);
n = 10;  // n x n x n  interpolation points
x = linspace(0,1,n); y=x; z=x; // interpolation grid
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);
if type(tl)<> 16 then pause,end
if size(tl)<> 7 then pause,end
if type(tl(1)) <> 10 then pause,end
if or(size(tl(2))<> [15 1]) then pause,end
if or(size(tl(3))<> [15 1]) then pause,end
if or(size(tl(4))<> [15 1]) then pause,end
if or(size(tl(6))<> [1000 1]) then pause,end
if or(size(tl(7))<> [6 1]) then pause,end
