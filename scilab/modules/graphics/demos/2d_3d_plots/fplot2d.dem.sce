// =============================================================================
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

my_plot_desc          = "fplot2d : f given by external";

my_handle             = scf(100001);
my_handle.figure_name = my_plot_desc;

fplot2d();
xtitle(" ","x ","f(x) ");
