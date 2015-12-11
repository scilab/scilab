// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_geom3d()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("geom3d.dem.sce");

    // DEMO START

    my_plot_desc          = "plot3d and use of geom3d";
    my_handle.figure_name = my_plot_desc;

    r = (%pi):-0.01:0;
    x = r.*cos(10*r);
    y = r.*sin(10*r);

    deff("[z]=Surf(x,y)","z=sin(x)*cos(y)");
    t=%pi*(-10:10)/10;

    // Plot the surface'
    fplot3d(t,t,Surf,35,45,"X@Y@Z",[19,2,3]);

    // Add 2d graphics on 3d graphic'
    z=(sin(x).*cos(y));
    [x1,y1] = geom3d(x,y,z);
    xpoly(x1,y1,"lines");

    // Adjust some plotting parameters'
    BackgroundColorId           = color(70,174,255);
    current_axe                 = gca();
    plot_3d                     = current_axe.children(2);
    plot_3d.hiddencolor         = 32;
    polyline                    = current_axe.children(1)';;
    polyline.foreground         = 8;
    current_axe.rotation_angles = [70,47];
    current_axe.background      = BackgroundColorId;

    // Add special point for datatip
    param3d(0, 0, 0);
    e = gce();
    dt = datatipCreate(e, 0);
    function str=mydisplay(h)
        pt = h.data;
        str = msprintf('The point\n (%0.2g,%0.2g,%0.2g)', pt(1), pt(2), pt(3))
    endfunction
    datatipSetDisplay(dt,"mydisplay")

    a = gca();
    a.rotation_angles = [75, 225];

    xtitle(my_plot_desc," "," "," ");

    // DEMO END
endfunction


demo_geom3d();
clear demo_geom3d;
