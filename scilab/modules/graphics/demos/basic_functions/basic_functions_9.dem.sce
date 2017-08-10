// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_basic_functions9()
    function [z]=__milk_drop(x,y)
        sq=x.^2+y.^2;
        z= exp( exp(-sq).*(exp(cos(sq).^20)+8*sin(sq).^20+2*sin(2*(sq)).^8) );
    endfunction
    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("basic_functions_9.dem.sce");
    my_plot_desc          = "milk drop";
    my_handle.figure_name = my_plot_desc;
    my_handle.color_map = jetcolormap(256);

    x = -2:0.1:2;
    y = x;
    [X,Y] = ndgrid(x,y);
    z = __milk_drop(X,Y);
    plot3d(x, y, z);
    e = gce();
    e.color_flag = 1;
    a = gca();
    a.rotation_angles = [25, 30];
    // DEMO END

endfunction

demo_basic_functions9();
clear demo_basic_functions9;
