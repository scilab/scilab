// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function [z]=RndIntT(n)

    function [y] = RndInt(m,n,imin,imax)
        // discrete uniform random number
        //-------------------------------
        y = rand(m, n, "uniform");
        y = int(floor(y*(imax+1-imin)+ imin ));
    endfunction

    [lhs,rhs] = argn(0)
    if rhs <= 0 ; n = 10000;end
    imin = -10;
    imax = 10;
    y = RndInt(1,n,-10,10);
    i = imin-2:imax+2;
    z = [];
    for i1=i, z=[z,prod(size(find(y==i1)))],end

    drawlater();
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("discrete_uni.dem.sce");
    plot2d3("onn",i',z'/n,[1,2],,,[-12,0,12,0.1]);
    i1=(imin:imax)';
    plot2d(i1, ones(i1)/prod(size(imin:imax)), [-2,5])

    xtitle(_("Simulation of a discrete uniform random variable"));
    current_axe = gca();
    current_axe.title.font_size = 3;
    current_axe.background = color(232,230,200);
    legend([_("Simulation");_("Theory")]);
    drawnow();

endfunction

RndIntT();
clear RndIntT;

