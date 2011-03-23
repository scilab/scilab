//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// tube : some tube like bodies.
// =============================================================================

function tube(nn)

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);

    x=linspace(0,2*%pi,nn);
    //  atomic modell or so.
    y=0.1+[sin(linspace(0,%pi,15)),1.5*sin(linspace(0,%pi,10)),sin(linspace(0,%pi,15))];
    cosphi=dup(cos(x),length(y));
    sinphi=dup(sin(x),length(y));
    f=dup(y',length(x));
    x1=f.*cosphi;     y1=f.*sinphi;
    z=dup(linspace(-2,2,prod(size(y)))',prod(size(x)));

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;

    demo_viewCode("tube.sce");

    my_handle.immediate_drawing = "off";
    plot3d2(x1,y1,z,-1,35,70);
    my_plot                     = my_axe.children;
    my_handle.color_map         = jetcolormap(128);
    my_plot.color_flag          = 1;
    my_axe.rotation_angles      = [70,48];
    my_handle.immediate_drawing = "on";

endfunction

tube(50);
clear tube;
