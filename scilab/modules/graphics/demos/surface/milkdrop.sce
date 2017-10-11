// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_milk_drop()
    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("milkdrop.sce");
    my_plot_desc          = "milk drop";
    my_handle.figure_name = my_plot_desc;
    my_handle.color_map = jetcolormap(256);

    x = -15:0.1:15;
    y = x;
    [X,Y] = ndgrid(x,y);
    R = sqrt(X.^2+Y.^2);
    // z= exp(-sq).*(exp(cos(sq).^20)+8*sin(sq).^20+2*sin(2*(sq)).^8);
    Z = cos(R).*exp(-R/5);

    plot3d(x, y, Z);
    e = gce();
    e.color_flag = 1;
    e.thickness = 0;
    a = gca();
    a.rotation_angles = [85 30];
    // DEMO END

endfunction

demo_milk_drop();
clear demo_milk_drop;
