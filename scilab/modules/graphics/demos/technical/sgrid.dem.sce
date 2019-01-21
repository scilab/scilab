// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_sgrid()
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("sgrid.dem.sce");
    my_plot_desc = "sgrid";
    my_handle.figure_name = my_plot_desc;

    // DEMO START
    sgrid();
    xtitle(my_plot_desc," "," "," ");
    // DEMO END
endfunction

demo_sgrid();
clear demo_sgrid;
