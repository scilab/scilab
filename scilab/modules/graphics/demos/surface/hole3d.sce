//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// hole3d : Holes in surfaces using %inf
// =============================================================================

function hole3d()

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("hole3d.sce");

    t     = linspace(-%pi,%pi,40);
    z     = sin(t)'*cos(t);
    z1    = find(abs(z) > 0.5);
    z(z1) = %inf*z1;
    plot3d1(t,t,z);

endfunction

hole3d();
clear hole3d;
