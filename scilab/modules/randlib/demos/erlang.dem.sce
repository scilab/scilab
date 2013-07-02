// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function [y]=ErlangT(n)

    function [y] = Erlang(m, n, pMean, pVariance)
        k = int( (pMean * pMean ) / pVariance + 0.5 );
        if (k <= 0) then k = 1;end
        a = k / pMean;
        // we use blocks of size 100 to avoid overflows
        res = [];
        ntir = 100;
        ntirc = ntir;
        y = rand(ntir, k, "uniform");
        y = -log(prod(y, "r"))/a;
        res = [res; y];
        while ( ntirc < m*n )
            y = rand(ntir, k, "uniform");
            y = -log(prod(y, "r"))/a;
            res = [res; y];
            ntirc = ntirc + ntir;
        end
        y = matrix(res(1:m*n), m, n);
    endfunction

    [lhs, rhs] = argn(0);
    if rhs <= 0 ; n = 10000;end
    y = Erlang(1, n, 10, 1);

    drawlater();
    my_handle = scf(100001);
    clf(my_handle, "reset");
    demo_viewCode("erlang.dem.sce");
    histplot(20, y, [1,1], "061");
    xtitle(_("Simulation of a Erlang random variable"));
    BackgroundColorId = color(232,230,200);
    PolylineFillColorId = color(179,179,0);
    current_axe = gca();
    current_axe.title.font_size = 3;
    current_axe.background = color(232,230,200);
    current_axe.children.children.fill_mode = "on";
    current_axe.children.children.background = PolylineFillColorId;
    current_axe.grid = [-1 PolylineFillColorId];
    drawnow();
endfunction

ErlangT();
clear Erlang;
clear ErlangT;
