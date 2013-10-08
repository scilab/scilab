// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function [] = WeibullT(n)

    function [y] = Weibull(m, n, Alpha, Beta)
        //-------------------------------
        y = rand(m, n, "uniform");
        y = (Beta*( - log(1-y))).^(1/Alpha);
    endfunction

    [lhs, rhs] = argn(0);
    if rhs <= 0 ; n = 10000;end
    y = Weibull(1, n, 1, 2);

    drawlater();
    my_handle = scf(100001);
    clf(my_handle, "reset");
    demo_viewCode("weibull.dem.sce");
    histplot(20, y, [1,1], "061");
    xtitle(_("Simulation of a Weibull random variable"));
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

WeibullT();
clear WeibullT;
