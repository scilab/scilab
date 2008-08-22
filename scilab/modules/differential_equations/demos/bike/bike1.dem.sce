//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("bike1.dem.sce");

exec(thispath+"macros/show.sci");
exec(thispath+"macros/velod.sci");
exec(thispath+"macros/velodp.sci");
exec(thispath+"macros/velo2.sci");

disp(thispath+"macros/show.sci");

x = read(thispath+"data/x1.dat",46,368,"(e10.4)");

my_handle = scf(100001);
clf(my_handle,"reset");
toolbar(my_handle.figure_id, "off");

my_axe                 = gca();
my_axe.title.font_size = 3;
my_axe.title.text      = "bike simulation, unstable trajectory";

show(x(:,300:368),1,1,velo4p);

// Clear variable and functions
clear my_axe x;
clear show get_velo_rti velo1 velo4p poteau arbre velodp velod;
