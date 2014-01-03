// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_basic_functions9()

    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("basic_functions_9.dem.sce");
    my_plot_desc          = "milk drop";
    my_handle.figure_name = my_plot_desc;

    expr=[
    "x = -2:0.1:2;"
    "y = x;"
    "z = eval3d(milk_drop, x, y);"
    "plot3d(x, y, z)"];
    execstr(expr);
    // DEMO END

endfunction

demo_basic_functions9();
clear demo_basic_functions9;
