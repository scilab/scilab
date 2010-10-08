//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// shell()
// =============================================================================

function shell()

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);
    u = linspace(0,2*%pi,40);
    v = linspace(0,2*%pi,20);
    x= (cos(u).*u)'*(1+cos(v)/2);
    y= (u/2)'*sin(v);
    z= (sin(u).*u)'*(1+cos(v)/2);

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;

    demo_viewCode("shell.sce");

    my_handle.immediate_drawing = "off";
    plot3d2(x,y,z);
    my_plot                     = my_axe.children;
    my_handle.color_map         = jetcolormap(128);
    my_plot.color_flag          = 1;
    my_axe.rotation_angles      = [51,96];
    my_handle.immediate_drawing = "on";

endfunction

shell();
clear shell;
