// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function []=GeomT(n)

    function [y]=Geom(m,n,p)
        // P(0)= 0 P(i) = p*(1-p)^{n-1} P(inf)=0
        // E = 1/p ; sig2= (1-p)/p^2
        //--------------------------------------
        if p >= 1 then disp(_("p must be < 1"));end
        y = 0*ones(m, n);
        for i=1:m*n,
            samples = 1;
            z = rand(1,1,"uniform");
            while( z < 1-p) ,z=rand(1,1,"uniform"); samples=samples+1;end
            y(i) = samples;
        end
        y = matrix(y, m, n);
    endfunction

    [lhs, rhs] = argn(0);
    if rhs <= 0 ; n = 10000;end
    pr = 0.2;
    y = Geom(1, n, pr);
    N = 20;
    i = 0:N;
    z = [];
    for i1 = i, z = [z, prod(size(find(y==i1)))], end

    drawlater();

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("geometric.dem.sce");
    plot2d3("onn", i', z'/n, [1,3]);
    zt = [0];
    for i1=1:N; zt=[zt,pr*(1-pr)^(i1-1)];end
    plot2d(i, zt', [-2,6])
    xtitle(_("Simulation of a geometric random variable"));
    current_axe = gca();
    current_axe.title.font_size = 3;
    current_axe.background = color(232,230,200);
    legend([_("Simulation");_("Theory")]);
    drawnow();
endfunction

GeomT();
clear GeomT;
