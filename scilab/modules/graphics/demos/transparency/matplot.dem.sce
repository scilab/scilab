// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_matplot()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("matplot.dem.sce");

    // DEMO START

    my_plot_desc          = _("Matplot with transparency");
    my_handle.figure_name = my_plot_desc;

    a = (1:64)' * ones(1, 64) + ones(64, 1) * (1:64);
    Matplot(a)
    e = gce();
    e.face_alpha          = 0.5;    // transparent image (FaceAlpha)
    colormap(my_handle, jet(128))

    // DEMO END

endfunction


demo_matplot();
clear demo_matplot;
