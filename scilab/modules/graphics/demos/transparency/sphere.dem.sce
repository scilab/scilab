// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_sphere()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("sphere.dem.sce");

    // DEMO START

    my_plot_desc          = _("Sphere & Cyliner");
    my_handle.figure_name = my_plot_desc;

    gcf().color_map = jet(64);

    [x, y, z] = cylinder(3, 40);
    hcyl = surf(x, y, 4 * z - 2, "FaceAlpha", 0.5, "EdgeAlpha", 0.0);
    hcyl.face_alpha = 0.5;

    [x, y, z] = sphere(40);
    hsphere = surf(2 * x, 2 * y, 2 * z, "FaceAlpha", 0.6, "EdgeAlpha", 0.2);

    // DEMO END

endfunction

demo_sphere();
clear demo_sphere;
