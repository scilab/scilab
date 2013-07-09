// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function [] = HyperGeomT(n)

    function [y] = HyperGeom(m, n, Mean, var)
        z = var / (Mean * Mean);
        pP = 0.5 * (1.0 - sqrt((z - 1.0) / ( z + 1.0 )));
        y = rand(m, n, "uniform")
        zz = find( y > pP) ;
        y = pP * ones(y);
        y(zz) = (1-pP) * ones(zz);
        y1 = rand(m, n, "uniform")
        y = -Mean * log(y1) ./ (2.0 * y) ;
    endfunction

    [lhs,rhs]=argn(0)
    if rhs <= 0 ; n=10000;end
    y=HyperGeom(1,n,1,10)

    drawlater();
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("hypergeom.dem.sce");
    histplot([0:0.25:10], y, [1,1], "061", " ", [0,0,10,0.4]);
    xtitle(_("Simulation of a hyper geometric random variable"));
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

HyperGeomT();
clear HyperGeomT;
