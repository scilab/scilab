//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// Show a Bezier surface

function beziersurftest

    x=linspace(-%pi,%pi,5);
    [x,y]=field(x,x);
    z= 3*sin(x).*cos(y);
    [xb,yb,zb]=beziersurface(x,y,z);

    my_handle = scf(100001);
    clf(my_handle,"reset");

    subplot(2,1,1);
    drawlater();
    plot3d3(x,y,z);
    title("A first surface","fontsize",3);
    subplot(2,1,2);
    plot3d2(xb,yb,zb,-1,35,45," ",[4,2,3]);
    title("The bezier interpolated surface (n=10)","fontsize",3);
    drawnow();
    demo_viewCode("beziersurftest.sce");

endfunction

beziersurftest();
clear beziersurftest;
