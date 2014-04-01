// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plot3d()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("plot3d.dem.sce");

    // DEMO START
    my_plot_desc          = "plot3d : z=sin(x)*cos(y)";
    my_handle.figure_name = my_plot_desc;
    plot3d();
    xtitle(my_plot_desc," "," "," ");
    // DEMO END

endfunction

demo_plot3d();
clear demo_plot3d;

