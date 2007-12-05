// =============================================================================
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

my_plot_desc          = "fplot3d : z=f(x,y), f external";

my_handle             = scf(100001);
my_handle.figure_name = my_plot_desc;

fplot3d();
xtitle(my_plot_desc," "," "," ");
