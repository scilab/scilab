//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

mode(-1);

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("basic_functions_4.dem.sce");

my_plot_desc          = "General functions";
my_handle.figure_name = my_plot_desc;

plot2d([-100,500],[-100,600],[-1,-1],"022");

a           = gca();
t           = a.title;
t.text      = my_plot_desc;
t.font_size = 3;

plot3d();

x_message(["[IV] General functions"]);

// Demo IV.1
expr=["scf()"
"plot2d()";
"a=gca(); t=a.title;"
"t.text=""window proporties""; t.font_size=3;"];
x_message(["[IV.1] Creating a new window";expr]);
execstr(expr);

// Demo IV.2
expr=["xset(''window'',0);";
"xselect();";]

execstr(expr);
