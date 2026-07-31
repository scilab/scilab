// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_polyline()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("polyline.dem.sce");

    // DEMO START

    my_plot_desc          = _("Polyline with transparency (background/foreground)");
    my_handle.figure_name = my_plot_desc;

    x = -2*%pi:(%pi/20):2*%pi;
    h = plot(x, cos(x), "-", "BackGround", 5, "BackgroundAlpha", 0.2, ..
        "ForegroundAlpha", 0.6);
    h.thickness = 2;

    h = plot(x-%pi, cos(x), "-.", "BackGround", 6, "BackgroundAlpha", 0.4, ..
        "ForegroundAlpha", 0.3);
    h.thickness = 2;
    
    // DEMO END

endfunction


demo_polyline();
clear demo_polyline;
