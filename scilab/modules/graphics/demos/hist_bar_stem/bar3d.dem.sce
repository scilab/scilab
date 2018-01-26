// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Samuel GOUGEON
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_bar3d()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("bar3d.dem.sce");

    // DEMO START
    my_plot_desc = "bar3d: Average monthly temperatures in french cities";
    my_handle.figure_name = my_plot_desc;

    bar3d();
    xtitle(my_plot_desc," "," "," ");
    // DEMO END

endfunction

demo_bar3d();
clear demo_bar3d;



