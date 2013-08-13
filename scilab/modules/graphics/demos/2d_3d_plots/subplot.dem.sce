// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_subplot()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("subplot.dem.sce");

    // DEMO START

    my_plot_desc          = "subplot";
    my_handle.figure_name = my_plot_desc;

    f = gcf();
    f.axes_size = [790, 570];
    subplot(2,2,1);
    plot3d();
    subplot(2,2,2);
    plot2d();
    subplot(2,2,3);
    histplot();
    subplot(2,2,4);
    grayplot();

    // DEMO END

endfunction

demo_subplot();
clear demo_subplot;
