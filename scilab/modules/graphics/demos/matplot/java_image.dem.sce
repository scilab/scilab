// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_java_image()
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("java_image.dem.sce");
    demopath = get_absolute_file_path("java_image.dem.sce");

    // DEMO START

    my_plot_desc          = _("Java Image");
    my_handle.figure_name = my_plot_desc;

    jimport javax.imageio.ImageIO;
    jimport java.net.URL;

    bi = ImageIO.read(URL.new("file:////" + demopath + "/puffin.png"));
    w = bi.getWidth(jvoid);
    h = bi.getHeight(jvoid);
    data = bi.getRGB(0, 0, w, h, [], 0, w);
    data = matrix(data, double(w), double(h));
    data = uint32(data');

    Matplot(data);
    e = gce();
    e.image_type="argb";

    // DEMO END
endfunction


demo_java_image();
clear demo_java_image;