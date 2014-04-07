//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 Scilab Enterprises - Pierre-Aime Agnel
//
// This file is distributed under the same license as the Scilab package.

// = =========================================================================== =
// xarrows
// = =========================================================================== =

fp_old = funcprot();
w_old  = warning("query");
funcprot(0);
warning("off");

function demo_basic_function_xarrows()
    f = figure("dockable", "off", "toolbar", "none", "visible", "off");
    f.backgroundcolor = [1, 1, 1];
    f.figure_name = "xarrows in 3D";

    N = 100;
    zi = linspace(-1, 2, N);
    zf = zi;
    theta = (zi - min(zi)) / (max(zi)-min(zi)) * 720;
    xi = zeros(1, N);  xf = cosd(theta);
    yi = zeros(1, N);  yf = sind(theta);

    plot3d([-1 1 1 1], [-1 -1 -1 1], [-1 -1 2 2])
    xarrows([xi; xf], [yi; yf], [zi; zf], 0.7, 32*rand(1, N))

    demo_viewCode("basic_functions_xarrows.dem.sce")
    f.visible = "on"
endfunction

demo_basic_function_xarrows();
clear demo_basic_function_xarrows;

funcprot(fp_old);
warning(w_old);
clear fp_old w_old;
