// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_svplot()
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("svplot.dem.sce");
    my_plot_desc = "svplot";
    my_handle.figure_name = my_plot_desc;

    // DEMO START
    x = logspace(-3,3);
    y = svplot(ssrand(2,2,4),x);
    clf()
    plot2d("ln", x, 20*log(y')/log(10))
    xgrid(12)
    xtitle("Singular values plot","(rad/sec)", "dB");
    // DEMO END
endfunction

demo_svplot();
clear demo_svplot;
