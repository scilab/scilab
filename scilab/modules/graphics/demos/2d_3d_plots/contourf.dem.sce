// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is released into the public domain

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("contourf.dem.sce");

// DEMO START

my_plot_desc          = "contourf";
my_handle.figure_name = my_plot_desc;

contourf();
xtitle(my_plot_desc," "," "," ");

// DEMO END
