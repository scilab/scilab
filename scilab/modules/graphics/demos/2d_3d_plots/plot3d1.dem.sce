// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plot3d1()

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("plot3d1.dem.sce");
    my_plot_desc = "plot3d1 : z=sin(x)*cos(y)";
    my_handle.figure_name = my_plot_desc;

    plot3d1();

    xtitle(my_plot_desc,"X","Y","Z");
endfunction

demo_plot3d1();
clear demo_plot3d1;
