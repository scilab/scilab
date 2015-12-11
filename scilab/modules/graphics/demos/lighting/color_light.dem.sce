// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Pedro SOUZA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_color_light()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("color_light.dem.sce");

    // DEMO START

    my_plot_desc          = "Color light";
    my_handle.figure_name = my_plot_desc;

    plot3d;l=light();s=gce();

    s.use_color_material = "off";
    s.color_flag = 0;
    s.thickness = 0;
    s.hiddencolor = -1;
    s.ambient_color = [1 1 1];
    s.diffuse_color = [1 1 1];
    s.specular_color = [1 1 1];

    diff0 = rand(1, 3);
    diff1 = rand(1, 3);
    spec0 = rand(1, 3);
    spec1 = rand(1, 3);

    num = 10;
    for j=1: num
        n = 700;
        for i=1:n
            if ~is_handle_valid(l) then
                break
            end
            t = i / n;
            cdiff = diff0 * (1 - t) + diff1 * t;
            cspec = spec0 * (1 - t) + spec1 * t;
            l.ambient_color = 0.2 * cdiff;
            l.diffuse_color = 0.8 * cdiff;
            l.specular_color = cspec;
            sleep(3);
        end
        if ~is_handle_valid(l) then
            break
        end
        diff0 = diff1;
        spec0 = spec1;
        diff1 = rand(1, 3);
        spec1 = rand(1, 3);
    end
    // DEMO END

endfunction


demo_color_light();
clear demo_color_light;
