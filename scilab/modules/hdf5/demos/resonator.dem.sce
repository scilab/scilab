// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - S/E - Sylvestre Ledru
// Copyright (C) 2013 - LASS - Antoine Monmayrant
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function resonator_demo()



    my_test_fig = figure(100001);
    my_test_fig.figure_name = _("Ring Resonator from a HDF5 file");

    filename = "resonator.dem.sce";
    demo_viewCode(filename);

    h5=h5open(SCI+"/modules/hdf5/demos/eps.h5","r");
    eps=h5.root.eps.data;

    surf(eps);
    h=gcf();
    h.color_map=hotcolormap(64);
    f=gcf();f.background=60;
    e=gce();
    a=gca();
    a.x_label.text=_("X position [a]");
    a.y_label.text=_("Y position [a]");
    a.z_label.text=_("Permittivity");
    a.z_label.font_angle=-90;
    e.thickness=0;
    a.rotation_angles=[20,-140];
    a.title.text=_("Ring Resonator");

endfunction

resonator_demo();
clear resonator_demo;
