// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Pedro SOUZA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_rot_light()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("rot_light.dem.sce");

    // DEMO START

    my_plot_desc          = "Rotating light";
    my_handle.figure_name = my_plot_desc;

    my_handle.figure_name = my_plot_desc;
    plot3d;l=light();s=gce();

    for i=0:10000
        sleep(5);
        x = 2*cos(i*%pi/180);
        y = 2*sin(i*%pi/180);
        z = 2;
        l.light_position = [x y z];
    end

    // DEMO END

endfunction


demo_rot_light();
clear demo_rot_light;

