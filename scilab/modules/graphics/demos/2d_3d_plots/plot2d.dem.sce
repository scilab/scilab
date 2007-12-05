// =============================================================================
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

my_plot_desc          = "plot2d and xgrid";

my_handle             = scf(100001);
my_handle.figure_name = my_plot_desc;

scf(100001);

t=(0:0.1:6*%pi);
plot2d(t',sin(t)');
xtitle(my_plot_desc,"t","sin(t)");
xgrid();
