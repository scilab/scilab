// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_scatter()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("scatter.dem.sce");

    // DEMO START

    my_plot_desc          = _("Scatter with transparency");
    my_handle.figure_name = my_plot_desc;

    x = 1:20;
    y = rand(1,20);
    sz = rand(1,20) * 5000
    scatter(x, y, sz, [0 0.5 0.75], "fill", "MarkerFaceAlpha", 0.3, "MarkerEdgeAlpha", 0.8)

    // DEMO END

endfunction


demo_scatter();
clear demo_scatter;
