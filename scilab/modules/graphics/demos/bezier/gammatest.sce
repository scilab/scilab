//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// Bezier curve approximating a circle

function gammatest (N)

    [lhs,rhs] =argn(0);

    if rhs < 1 then
        N=[10,20,50,100];
    end

    x      = linspace(0,1,100);

    my_handle = scf(100001);
    clf(my_handle,"reset");

    plot2d(cos(2*%pi*x),sin(2*%pi*x));
    isoview()
    ax     = gca();
    title("Bezier curve approximating a circle","fontsize",3);
    icol=2;

    for n=N ;
        t = sqrt(linspace(0,1,n));
        p = [cos(2*%pi*t);sin(2*%pi*t)];
        y = bezier(p,x);
        plot2d(y(1,:),y(2,:),icol);
        icol=icol+1;
    end

    demo_viewCode("gammatest.sce");

endfunction

gammatest();
clear gammatest;
