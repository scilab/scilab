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

// <-- BENCH NB RUN : 3 -->

function R = computeMandelbrot(xsize,ysize,nmax,xmin,xmax,ymin,ymax)
    xvect = linspace( xmin, xmax, xsize );
    yvect = linspace( ymin, ymax, ysize );
    R = zeros(xsize,ysize);
    for i = 1:xsize
        for j = 1:ysize
            x = xvect(i);
            y = yvect(j);
            x0 = x;
            y0 = y;
            k = 0;
            while( x*x + y*y < 4 & k < nmax )
                xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                k=k+1;
            end
            if k<nmax then
                R(i,j) = k;
            else
                R(i,j) = -1;
            end
        end
    end
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

