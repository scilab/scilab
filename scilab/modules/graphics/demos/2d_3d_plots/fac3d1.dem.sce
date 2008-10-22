//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("fac3d1.dem.sce");

// DEMO START

my_plot_desc          = "fac3d1";
my_handle.figure_name = my_plot_desc;

fac3d1();

xtitle(my_plot_desc," "," "," ");

// DEMO END
