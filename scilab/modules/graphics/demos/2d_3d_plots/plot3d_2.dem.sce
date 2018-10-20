// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plot3d_2()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("plot3d_2.dem.sce");

    // DEMO START

    my_plot_desc          = "plot3d";
    my_handle.figure_name = my_plot_desc;

    deff("[x,y,z]=sph(alp,tet)",["x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)"; ..
    "y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)"; ..
    "z=r*sin(alp)+orig(3)*ones(tet)"]);
    r = 1;
    orig = [0 0 0];
    [x1,y1,z1] = eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20));
    [n1,m1] = size(x1);
    r = 1/2;
    orig = [-1 0 0];
    [x2,y2,z2] = eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20));
    [n2,m2] = size(x2);
    x = [x1 x2];
    y = [y1 y2];
    z = [z1 z2];
    plot3d(x,y,z);
    gca().rotation_angles = [58 112];
    isoview

    xtitle(my_plot_desc," "," "," ");

    // DEMO END
endfunction

demo_plot3d_2();
clear demo_plot3d_2;
