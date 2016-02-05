// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Pedro SOUZA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_rot_light()

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("rot_light.dem.sce");

    // DEMO START
    my_plot_desc          = "Rotating light";
    my_handle.figure_name = my_plot_desc;

    plot3d;
    l=light();
    s=gce();

    for i=0:2000
        sleep(5);
        x = 2*cos(i*%pi/180);
        y = 2*sin(i*%pi/180);
        z = 2;
        if is_handle_valid(l) then
            l.position = [x y z];
        else
            break
        end
    end
    // DEMO END
endfunction


demo_rot_light();
clear demo_rot_light;

