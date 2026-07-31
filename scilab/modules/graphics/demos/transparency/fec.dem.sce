// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_fec()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("fec.dem.sce");

    // DEMO START

    my_plot_desc          = _("Fec with transparency");
    my_handle.figure_name = my_plot_desc;

    // Triangular mesh of a disk
    N = 20; n = 1:N;
    x = cos(n * 2 * %pi / N);
    y = sin(n * 2 * %pi / N);
    nodes = [(1:N)', x', y', zeros(N, 1) ; (N + 1), 0, 0, 0];
    triangles = [];
    for i = 1:(N - 1)
        triangles = [triangles ; i, i, i + 1, N + 1, 0];
    end
    triangles = [triangles ; N, N, 1, N + 1, 0];

    func = (1:N + 1)';
    fec(nodes(:, 2), nodes(:, 3), triangles, func)
    e = gce();
    e.face_alpha          = 0.5;    // transparent triangles (FaceAlpha)
    colormap(my_handle, jet(128))

    gca().axes_visible = ["off","off","off"];

    // DEMO END

endfunction


demo_fec();
clear demo_fec;
