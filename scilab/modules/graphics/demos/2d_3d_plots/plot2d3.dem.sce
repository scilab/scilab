// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plot2d3()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("plot2d3.dem.sce");

    // DEMO START

    my_plot_desc          = "plot2d3 (vbar plot)";
    my_handle.figure_name = my_plot_desc;

    n = 32-1;
    t = (0:n)/n;
    u = sin(80*%pi*t)+sin(100*%pi*t);

    plot2d3("enn",1,abs(fft(u,-1))'/n);
    xtitle(my_plot_desc,"t","f(t)");

    // DEMO END

endfunction

demo_plot2d3();
clear demo_plot2d3;
