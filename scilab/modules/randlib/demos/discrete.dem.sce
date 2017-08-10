// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function [z] = RndDiscT(n)

    function [y] = RndDisc(m,n,x,p)
        // discrete law random number
        // sum p_i delta_{x_i}
        //-------------------------------
        p1 = [0, p];
        p1 = cumsum(p1);
        y = rand(m, n, "uniform");
        N = prod(size(x));
        res = 0*ones(m*n);
        for i=1:N,z=0*ones(m*n,1),id=find( p1(i) <= y & y < p1(i+1) ),
            z(id) = x(i)*ones(prod(size(id))),res=res+z;
        end
        y = matrix(res, m, n);
    endfunction

    [lhs,rhs] = argn(0)
    if rhs <= 0 ; n = 10000;end
    x = [1,3,4,6,10,12];
    pr = [0.1,0.2,0.3,0.2,0.1,0.1];
    y = RndDisc(1,n,x,pr);
    i = 0:13
    z = [];
    for i1=i, z=[z,prod(size(find(y==i1)))],end

    drawlater();

    my_handle = scf(100001);
    clf(my_handle, "reset");
    demo_viewCode("discrete.dem.sce");

    plot2d3("onn",i',z'/n,[1,3],,,[0,0,14,0.5]);
    plot2d(x, pr', [-2,6])
    xtitle(_("Simulation of a discrete random variable"));
    current_axe = gca();
    current_axe.title.font_size = 3;
    current_axe.background = color(232,230,200);
    legend([_("Simulation"); _("Theory")]);

    drawnow();
endfunction

RndDiscT();
clear RndDiscT;

