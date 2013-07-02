//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// bh()
// =============================================================================


function bh(nn)

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);

    // a black hole

    x         = linspace(0,2*%pi,nn);
    t         = linspace(0,1,20);
    cosphi    = dup(cos(x),length(t));
    sinphi    = dup(sin(x),length(t));
    f         = dup((t.*t+0.2)',length(x));

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;
    demo_viewCode("bh.sce");


    my_handle.immediate_drawing = "off";
    plot3d2(f.*cosphi,f.*sinphi,dup(t'.*2-1,length(x)));
    my_plot                     = my_axe.children;
    my_handle.color_map         = jetcolormap(128);
    my_plot.color_flag          = 1;
    my_axe.rotation_angles      = [70,20];
    my_handle.immediate_drawing = "on";

endfunction

bh(50);
clear bh;
