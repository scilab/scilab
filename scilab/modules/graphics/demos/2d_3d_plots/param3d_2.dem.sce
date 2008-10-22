//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("param3d_2.dem.sce");

// DEMO START

my_plot_desc          = "param3d: parametric curves in R3 (t.sin(t),t.cos(t),t.|t|/50.pi)";
my_handle.figure_name = my_plot_desc;

t = -50*%pi:0.1:50*%pi;
x = t.*sin(t);
y = t.*cos(t);
z = t.*abs(t)/(50*%pi);

param3d(x,y,z,45,60);
xtitle(my_plot_desc," "," "," ");

// DEMO END
