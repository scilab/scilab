// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_markers()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("markers.dem.sce");

    // DEMO START

    my_plot_desc          = _("Markers with transparency");
    my_handle.figure_name = my_plot_desc;

    x = sin(0:(%pi/20):2*%pi);
    plot(x, "o", "MarkerFaceColor", [0 0 1], "MarkBackgroundAlpha", 0.2, ..
        "MarkerEdgeColor", [1 0 0], "MarkForegroundAlpha", 0.8, ..
        "MarkSize", 10);
    
    // DEMO END

endfunction


demo_markers();
clear demo_markers;
