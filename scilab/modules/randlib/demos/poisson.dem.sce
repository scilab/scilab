// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function [z]=PoissonT(n)

    function [y]=Poisson(m,n,pmean)
        // P{n} = exp(-lambda)lambda^n/n!
        // pmean =lambda
        //----------------------------
        y=0*ones(m,n)
        bound= exp(-pmean);
        for i=1:m*n,
            count = 0;
            lprod = 1;
            while( lprod >= bound), lprod = lprod*rand(1,1,"uniform");
                count = count + 1;
            end
            y(i) = count - 1;
        end
        y = matrix(y, m, n);
    endfunction


    [lhs, rhs] = argn(0);
    if rhs <= 0 ; n = 1000;end
    pmean = 3;
    y = Poisson(1, n, pmean);
    N = 20;
    i = 0:N;
    z = [];
    for i1=i, z=[z,prod(size(find(y==i1)))],end

    drawlater();
    my_handle = scf(100001);
    clf(my_handle, "reset");
    demo_viewCode("poisson.dem.sce");

    plot2d3("onn",i',z'/n,1);
    deff("[y]=fact(n)","if n==0 then y=1;else y=n*fact(n-1);end");
    zt=[];for i1=0:N; zt=[zt, exp(-pmean) *pmean^i1/fact(i1)];end
    plot2d(i, zt', [-2,6])
    xtitle(_("Simulation of a Poisson random variable"));
    current_axe = gca();
    current_axe.title.font_size = 3;
    current_axe.background = color(232,230,200);
    legend([_("Simulation");_("Theory")]);
    drawnow();

endfunction

PoissonT();
clear PoissonT;
