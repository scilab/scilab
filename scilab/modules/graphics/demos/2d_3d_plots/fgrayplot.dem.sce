// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_fgrayplot()

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("fgrayplot.dem.sce");
    my_plot_desc = "fgrayplot";
    my_handle.figure_name = my_plot_desc;

    fgrayplot();

    xtitle(my_plot_desc,"X","Y","Z");
endfunction

demo_fgrayplot();
clear demo_fgrayplot;
