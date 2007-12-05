// =============================================================================
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

my_plot_desc          = "subplot";

my_handle             = scf(100001);
my_handle.figure_name = my_plot_desc;

subplot(2,2,1);
plot3d();
subplot(2,2,2);
plot2d();
subplot(2,2,3);
histplot();
subplot(2,2,4);
grayplot();

xtitle(my_plot_desc," "," "," ");
