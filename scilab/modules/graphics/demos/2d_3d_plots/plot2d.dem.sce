// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plot2d()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("plot2d.dem.sce");

    // DEMO START

    my_plot_desc          = "plot2d and xgrid";
    my_handle.figure_name = my_plot_desc;

    t=(0:0.1:6*%pi);
    plot2d(t',sin(t)');
    xtitle(my_plot_desc,"t","sin(t)");
    xgrid(color("grey"));

    // DEMO END

endfunction

demo_plot2d();
clear demo_plot2d;