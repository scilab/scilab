// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
// Copyright (C) 2011 - DIGITEO - Mickael BAUDIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Naive mandelbrot benchmark
//==============================================================================

// <-- BENCH NB RUN : 30 -->

function R = computeMandelbrot(xsize,ysize,nmax,xmin,xmax,ymin,ymax)
    xvect = linspace( xmin, xmax, xsize );
    yvect = linspace( ymin, ymax, ysize );
    [X,Y]=meshgrid(xvect,yvect);

    Z = zeros(xsize,ysize);
    R = -ones(xsize,ysize);
    W = zeros(xsize,ysize);
    C=X+%i*Y;
    J = 1:xsize*ysize;
    for k=0:nmax
        L = J(J>0);
        Z(L) = Z(L).^2+C(L);
        W(L) = abs(Z(L));
        M = find(W(L)>2);
        R(L(M)) = k;
        J(L(M)) = 0;
    end
    R = R';
endfunction

xsize = 50;
ysize = 50;
nmax = 1000;
xmin = 0.2675;
xmax = 0.2685;
ymin = 0.591;
ymax = 0.592;

// <-- BENCH START -->

R = computeMandelbrot(xsize,ysize,nmax,xmin,xmax,ymin,ymax);

// <-- BENCH END -->

