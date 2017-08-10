//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// sphere()
// =============================================================================

function sphere()

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);

    u = linspace(-%pi/2,%pi/2,40);
    v = linspace(0,2*%pi,20);
    x = cos(u)'*cos(v);
    y = cos(u)'*sin(v);
    z = sin(u)'*ones(v);

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;

    demo_viewCode("sphere.sce");

    my_handle.immediate_drawing = "off";
    plot3d2(x,y,z);
    isoview()
    my_plot                     = my_axe.children;
    my_handle.color_map         = jetcolormap(128);
    my_plot.color_flag          = 1;
    my_axe.rotation_angles      = [51,96];
    my_handle.immediate_drawing = "on";

endfunction

sphere();
clear sphere;
