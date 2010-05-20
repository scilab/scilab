// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is released into the public domain

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("plot2d1_1.dem.sce");

// DEMO START

my_plot_desc          = "plot2d1 log scale";
my_handle.figure_name = my_plot_desc;

plot2d1("enl",1,(1:10:10000)');
xtitle(my_plot_desc,"t","y  log scale");
xgrid(3);

// DEMO END
