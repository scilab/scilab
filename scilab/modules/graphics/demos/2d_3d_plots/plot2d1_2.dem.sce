// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is released into the public domain

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("plot2d1_2.dem.sce");

// DEMO START

my_plot_desc          = "plot2d1";
my_handle.figure_name = my_plot_desc;

rand("normal")
v=(1:20)+(1:20).*rand(1,20);

plot2d1("enn",1,v');
plot2d1("enn",1,(1:20)',[2,-2],"100","estimated");
xtitle(my_plot_desc," "," "," ");

// DEMO END
