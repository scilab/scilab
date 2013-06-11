//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

funcprot(0);

function [X,Y]=field(x,y)
    // x and y are two vectors defining a grid
    // X and Y are two matrices which gives the grid point coordinates
    //-------------------------------------------------------------
    [rx,cx]=size(x);
    [ry,cy]=size(y);
    if rx<>1, write(%io(2),"x must be a row vector");return;end;
    if ry<>1, write(%io(2),"y must be a row vector");return;end;
    X=x.*.ones(cy,1);
    Y=y'.*.ones(1,cx);
endfunction

function [z]=dup(x,n)
    // utility
    // x is a vector this function returns [x,x,x,x...] or [x;x;x;x;..]
    // depending on x
    [nr,nc]=size(x)
    if nr==1 then
        y=ones(n,1);
        z= x.*.y ;
    else
        if nc<>1 then
            error("dup : x must be a vector");
        else
            y=ones(1,n);
            z= x.*.y ;
        end
    end
endfunction

function cplxmap(z,w,varargin)
    //cplxmap(z,w,T,A,leg,flags,ebox)
    //cplxmap Plot a function of a complex variable.
    //       cplxmap(z,f(z))
    x = real(z);
    y = imag(z);
    u = real(w);
    v = imag(w);
    M = max(u);
    m = min(u);
    s = ones(size(z));
    //mesh(x,y,m*s,blue*s);
    //hold on
    [X,Y,U]=nf3d(x,y,u);
    [X,Y,V]=nf3d(x,y,v);
    Colors = sum(V,"r");
    Colors = Colors - min(Colors);
    Colors = 32*Colors/max(Colors);
    plot3d1(X,Y,list(U,Colors),varargin(:))
endfunction

function cplxroot(n,m,varargin)
    //cplxroot(n,m,T,A,leg,flags,ebox)
    //CPLXROOT Riemann surface for the n-th root.
    //       CPLXROOT(n) renders the Riemann surface for the n-th root.
    //       CPLXROOT, by itself, renders the Riemann surface for the cube root.
    //       CPLXROOT(n,m) uses an m-by-m grid.  Default m = 20.
    // Use polar coordinates, (r,theta).
    // Cover the unit disc n times.
    [lhs,rhs]=argn(0)
    if rhs  < 1, n = 3; end
    if rhs  < 2, m = 20; end
    r = (0:m)'/m;
    theta = - %pi*(-n*m:n*m)/m;
    z = r * exp(%i*theta);
    s = r.^(1/n) * exp(%i*theta/n);
    cplxmap(z,s,varargin(:))
endfunction

funcprot(1);
