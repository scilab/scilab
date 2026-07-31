// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plot3d()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("plot3d.dem.sce");

    // DEMO START

    my_plot_desc          = _("Transparent surface with transparent mesh");
    my_handle.figure_name = my_plot_desc;

    t = linspace(-%pi, %pi, 40);
    z = 5 * (sin(t)' * cos(t));
    plot3d1(t, t, z)
    e = gce();
    e.face_alpha          = 0.7;    // transparent faces (FaceAlpha)
    e.foreground_alpha    = 0.3;    // transparent mesh (EdgeAlpha)
    colormap(my_handle, jet(64))

    // DEMO END

endfunction


demo_plot3d();
clear demo_plot3d;
