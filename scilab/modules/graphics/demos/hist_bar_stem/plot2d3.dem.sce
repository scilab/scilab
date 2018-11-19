// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plot2d3()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("plot2d3.dem.sce");

    // DEMO START

    my_plot_desc          = "plot2d3 (stems)";
    my_handle.figure_name = my_plot_desc;

    n = 32-1;
    t = (0:n)/n;
    u = sin(80*%pi*t)+sin(100*%pi*t);

    drawlater
    plot2d3("enn",1,abs(fft(u,-1))'/n);
    xgrid(color("gold"),1,8);
    xtitle("","t","f(t)");
    title(my_plot_desc, "fontsize",3)
    curve = gca().children.children;

    set(curve, "foreground", color("limegreen"), "thickness", 1.5);
    curve.mark_mode = "on";
    set(curve, "mark_foreground", color("orange"), "mark_style", 10, "mark_size", 1);
    set(curve, "mark_stride", 2, "mark_offset", 1);

    drawnow

    // DEMO END
endfunction

demo_plot2d3();
clear demo_plot2d3;
