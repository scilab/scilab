// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

deff("z=f(x,y)","z = 1+ 50*(x.*(1-x).*y.*(1-y)).^2")
x = linspace(0,1,10);
[X,Y] = ndgrid(x,x);
X = X(:); Y = Y(:); Z = f(X,Y);
S = cshep2d([X Y Z]);
m = 40;
xx = linspace(-1.5,0.5,m);
[xp,yp] = ndgrid(xx,xx);
zp = eval_cshep2d(xp,yp,S);
if or(size(zp)<> [40 40]) then pause,end
