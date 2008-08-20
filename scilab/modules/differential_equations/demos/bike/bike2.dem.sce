//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("bike2.dem.sce");

exec(thispath+"macros/show.sci");
exec(thispath+"macros/velod.sci");
exec(thispath+"macros/velo1.sci");

x = read("SCI/modules/differential_equations/demos/bike/data/"+"xdemo.dat",46,401,"(e10.4)");

my_handle = scf(100001);
clf(my_handle,"reset");
toolbar(my_handle.figure_id, "off");

my_axe = gca();
my_axe.title.font_size = 3;
my_axe.title.text = 'bike simulation, stable trajectory';

show(x(:,1:340),1.6,1.47,velo1);
