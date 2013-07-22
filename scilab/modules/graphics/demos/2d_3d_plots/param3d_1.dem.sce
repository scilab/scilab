// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_param3d_1()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("param3d_1.dem.sce");

    // DEMO START

    my_plot_desc          = "param3d : parametric curves in R3";
    my_handle.figure_name = my_plot_desc;

    param3d();
    xtitle(my_plot_desc," "," ");

    // DEMO END
endfunction

demo_param3d_1();
clear demo_param3d_1
