//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// spiral()
// =============================================================================

function hole3d1()

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);
    [r,a]=field(0:0.1:1,0:%pi/8:6*%pi);
    z=a/8;
    x=r.*cos(a).*(1-a/20);
    y=r.*sin(a).*(1-a/20);
    z=z-1.5;

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;

    demo_viewCode("spiral.sce");

    my_handle.immediate_drawing = "off";
    plot3d2(x,y,z);
    my_plot                     = my_axe.children;
    my_handle.color_map         = jetcolormap(128);
    my_plot.color_flag          = 1;
    my_axe.rotation_angles      = [51,96];
    my_handle.immediate_drawing = "on";

endfunction

hole3d1();
clear hole3d1;
