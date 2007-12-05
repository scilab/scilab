// =============================================================================
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

my_plot_desc          = "champ";

my_handle             = scf(100001);
my_handle.figure_name = my_plot_desc;

champ();
xtitle(my_plot_desc," "," "," ");
