// =============================================================================
// Scilab Project
// Copyright INRIA 2007
// =============================================================================
	
	my_plot_desc          = "General functions";
	
	my_handle             = scf(100001);
	my_handle.figure_name = my_plot_desc;
	
	plot2d([-100,500],[-100,600],[-1,-1],"022");
	
	a           = gca();
	t           = a.title;
	t.text      = my_plot_desc;
	t.font_size = 3;
	
	plot3d();
	
	x_message(["[IV] General functions"]);
	
	// Demo IV.1
	expr=["xinit(display);";
	"plot2d()";
	"a=gca(); t=a.title;"
	"t.text=""window proporties""; t.font_size=3;"];
	x_message(["[IV.1] Creating a new window";expr]);
	execstr(expr);
	
	// Demo IV.2
	expr=["xset(''window'',0);";
	"xselect();";]
	x_message(["[IV.2] Selecting and raising window 0]";expr]);
	execstr(expr);
	