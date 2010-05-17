//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("contour.dem.sce");

// DEMO START

my_plot_desc          = "contour";
my_handle.figure_name = my_plot_desc;

contour();
xtitle(my_plot_desc," "," "," ");

// DEMO END
