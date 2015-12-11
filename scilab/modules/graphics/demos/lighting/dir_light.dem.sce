// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Pedro SOUZA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_dir_light()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("dir_light.dem.sce");

    // DEMO START

    my_plot_desc          = "Directional light";
    my_handle.figure_name = my_plot_desc;

    plot3d;l=light();s=gce();

    l.light_type = "directional";
    l.direction = [-1 0 -1];
    l.ambient_color = [0.1 0.1 0.1];
    s.diffuse_color = [0.5 0.4 0.4];
    s.use_color_material = "off";
    s.color_flag = 0;
    s.thickness = 0;
    s.hiddencolor = -1;
    s.specular_color = [0.6 0.6 1];

    // DEMO END
endfunction


demo_dir_light();
clear demo_dir_light;


