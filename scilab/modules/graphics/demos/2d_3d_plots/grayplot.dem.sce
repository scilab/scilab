// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_grayplot()

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("grayplot.dem.sce");

    my_plot_desc          = "grayplot";
    my_handle.figure_name = my_plot_desc;
    grayplot();
    xtitle(my_plot_desc," "," "," ");

endfunction

demo_grayplot();
clear demo_grayplot;
