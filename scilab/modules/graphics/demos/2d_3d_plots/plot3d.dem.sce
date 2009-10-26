//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("plot3d.dem.sce");
// DEMO START
my_plot_desc          = "plot3d : z=sin(x)*cos(y)";
my_handle.figure_name = my_plot_desc;
plot3d();
xtitle(my_plot_desc," "," "," ");
// DEMO END
