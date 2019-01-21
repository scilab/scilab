// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_bode()
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("bode.dem.sce");
    my_plot_desc = "bode";
    my_handle.figure_name = my_plot_desc;

    // DEMO START
    bode();
    xtitle(my_plot_desc," "," "," ");
    // DEMO END
endfunction

demo_bode();
clear demo_bode;
