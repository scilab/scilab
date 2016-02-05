// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_classical()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("classical.dem.sce");

    // DEMO START

    my_plot_desc          = _("Classical Matplot");
    my_handle.figure_name = my_plot_desc;

    Matplot();

    // DEMO END

endfunction


demo_classical();
clear demo_classical;
