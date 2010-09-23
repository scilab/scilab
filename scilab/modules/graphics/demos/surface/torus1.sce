//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// torus1 : a deformed torus
// =============================================================================

function torus1

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);
    x=linspace(0,2*%pi,40);
    y=linspace(0,2*%pi,20)';
    my_factor=1.5+cos(y);
    X=my_factor*cos(x);
    Y=my_factor*sin(x);
    Z=sin(y)*ones(x)+ ones(y)*cos(2*x);

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;
    demo_viewCode("torus1.sce");

    my_handle.immediate_drawing = "off";
    plot3d2(X,Y,Z);
    my_plot                     = my_axe.children;
    my_handle.color_map         = jetcolormap(128);
    my_plot.color_flag          = 1;
    my_axe.rotation_angles      = [30,110];
    my_handle.immediate_drawing = "on";

endfunction

torus1();
clear torus1;
