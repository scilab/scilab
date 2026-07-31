// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_grayplot()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("grayplot.dem.sce");

    // DEMO START

    my_plot_desc          = _("Grayplot with transparency");
    my_handle.figure_name = my_plot_desc;

    t = linspace(-2*%pi, 2*%pi, 80);
    z = sin(t)' * cos(t);
    grayplot(t, t, z)
    e = gce();
    e.face_alpha          = 0.5;    // transparent cells (FaceAlpha)
    colormap(my_handle, jet(64))

    gca().axes_visible = ["off","off","off"];

    // DEMO END

endfunction


demo_grayplot();
clear demo_grayplot;
