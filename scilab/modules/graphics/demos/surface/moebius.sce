//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// moebius()
// =============================================================================

function moebius()

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);
    // the Moebius band
    t=linspace(-1,1,20)';
    x=linspace(0,%pi,40);
    my_factor=2+ t*cos(x);
    X=my_factor*diag(cos(2*x));
    Y=my_factor*diag(sin(2*x));
    Z=t*sin(x);

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;

    demo_viewCode("moebius.sce");

    my_handle.immediate_drawing = "off";
    plot3d2(X,Y,Z);
    my_plot                     = my_axe.children;
    my_handle.color_map         = jetcolormap(128);
    my_plot.color_flag          = 1;
    my_axe.rotation_angles      = [88,56];
    my_handle.immediate_drawing = "on";


endfunction

moebius();
clear moebius;
