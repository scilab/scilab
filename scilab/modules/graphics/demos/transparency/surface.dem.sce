// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_surface()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("surface.dem.sce");

    // DEMO START

    my_plot_desc          = _("Transparent surface with transparent mesh");
    my_handle.figure_name = my_plot_desc;

    [x, y, z] = peaks(-4:0.2:4);
    s = surf(x, y, z, "FaceAlpha", 0.8, "EdgeAlpha", 0.1)
    colormap(my_handle, jet(64))

    // DEMO END

endfunction


demo_surface();
clear demo_surface;
