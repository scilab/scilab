//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// hole3d1 : Holes in surfaces using %inf
// =============================================================================

function hole3d1()

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);

    deff("[x,y,z]=sph(alp,tet)",["x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)";
    "y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)";
    "z=r*sin(alp)+orig(3)*ones(tet)"]);

    r=1;
    orig=[0 0 0];
    x=linspace(-%pi/2,%pi/2,40);y=linspace(0,%pi*2,20);
    x(5:8)=%inf*ones(5:8);
    x(30:35)=%inf*ones(30:35);
    [x1,y1,z1]=eval3dp(sph,x,y);

    my_handle                   = scf(100001);
    clf(my_handle,"reset");
    my_axe                      = my_handle.children;

    demo_viewCode("hole3d1.sce");

    my_handle.immediate_drawing = "off";
    plot3d1(x1,y1,z1);
    my_axe.rotation_angles      = [70,20];
    my_handle.immediate_drawing = "on";

endfunction

hole3d1();
clear hole3d1;
