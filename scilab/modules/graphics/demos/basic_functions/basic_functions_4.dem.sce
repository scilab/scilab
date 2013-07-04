// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_basic_function4()

    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");


    my_plot_desc          = "General functions";
    my_handle.figure_name = my_plot_desc;

    plot2d([-100,500],[-100,600],[-1,-1],"022");

    a           = gca();
    t           = a.title;
    t.text      = my_plot_desc;
    t.font_size = 3;

    plot3d();

    messagebox(["[IV] General functions"],"modal");

    // Demo IV.1
    expr=["scf(100001)"
    "plot2d()";
    "a=gca(); t=a.title;"
    "t.text=""window properties""; t.font_size=3;"];
    messagebox(["[IV.1] Creating a new window";expr],"modal");
    execstr(expr);

    // Demo IV.2
    expr=["scf(100001); fec();";
    "show_window();";]
    messagebox(["[IV.2] Creating window number 0";expr],"modal");
    execstr(expr);
    demo_viewCode("basic_functions_4.dem.sce");

endfunction

demo_basic_function4();
clear demo_basic_function4;
