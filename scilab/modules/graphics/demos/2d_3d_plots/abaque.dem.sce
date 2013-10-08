// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_abaque()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    // DEMO START
    my_plot_desc          = "chart";
    if is_handle_valid(my_handle) then
        my_handle.figure_name = my_plot_desc;
        nicholschart();
        xtitle(my_plot_desc," "," "," ");
    end
    demo_viewCode("abaque.dem.sce");
    // DEMO END

endfunction


demo_abaque();
clear demo_abaque;

