//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// torus : a torus with a thick and a thin side.
// =============================================================================

function torus

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);

    // some torus type bodies.
    x=linspace(0,2*%pi,40);
    y=linspace(0,2*%pi,20)';
    // a torus with a thick and a thin side.
    my_factor=1.5+cos(y)*(cos(x)/2+0.6);
    X=my_factor*diag(cos(x));
    Y=my_factor*diag(sin(x));
    Z=sin(y)*(cos(x)/2+0.6);

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;

    demo_viewCode("torus.sce");

    my_handle.immediate_drawing = "off";
    plot3d2(X,Y,Z);
    my_plot                     = my_axe.children;
    my_handle.color_map         = jetcolormap(128);
    my_plot.color_flag          = 1;
    my_axe.rotation_angles      = [68,95];
    my_handle.immediate_drawing = "on";

endfunction

torus();
clear torus;
