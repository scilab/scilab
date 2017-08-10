// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function [zt]=BinomialT(n)

    function [y]=Binomial(m,n,pb,nb)
        // Binomial law (p,N)
        // P{X=n} = C_N^n p^n (1-p)^(N-n)
        //----------------------------------
        res = [];
        // we use blocks of size 100 to avoid overflows
        ntir = 100;
        ntirc = ntir;
        y = rand(ntir,nb,"uniform");
        indy = find( y < pb);
        y = 0*ones(y);
        y(indy) = 1;
        y = sum(y,"c")
        res = [res;y];
        while ( ntirc < m*n )
            y = rand(ntir,nb,"uniform");
            indy = find(y< pb);
            y = 0*ones(y);
            y(indy) = 1;
            y = sum(y,"c")
            res = [res;y];
            ntirc = ntirc + ntir;
        end
        y = matrix(res(1:m*n),m,n);
    endfunction


    [lhs, rhs] = argn(0)
    if rhs <= 0 ; n=10000;end
    prb = 0.5;
    N = 10;
    y = Binomial(1, n, prb, N);
    i = 0:10;
    z = [];
    for i1=i, z=[z,prod(size(find(y==i1)))],end

    drawlater();

    my_handle = scf(100001);
    clf(my_handle, "reset");
    demo_viewCode("binomial.dem.sce");
    plot2d3("onn",i',z'/n,[1,3]);
    deff("[y]=fact(n)", "y=prod(1:n)");
    deff("[z]=C(N,n)", "z= fact(N)/(fact(n)*fact(N-n))");
    i = 0:N;
    zt = [];
    for j=i, zt=[zt, C(N,j)*prb^j*(1-prb)^(N-j)];end
    plot2d(i, zt', [-2,6])
    xtitle(_("Simulation of a binomial random variable"));
    current_axe = gca();
    current_axe.title.font_size = 3;
    current_axe.background = color(232,230,200);
    legend([_("Simulation");_("Theory")]);

    drawnow();

endfunction

BinomialT();
clear BinomialT;

