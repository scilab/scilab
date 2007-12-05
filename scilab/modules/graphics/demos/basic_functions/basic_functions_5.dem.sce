// =============================================================================
// Scilab Project
// Copyright INRIA 2007
// =============================================================================
	
	my_plot_desc          = "set of rectangles";
	
	my_handle             = scf(100001);
	my_handle.figure_name = my_plot_desc;
	
	// Demo V
	expr=[
	"n=16;"
	"rect=[-n,-n,n,n];"
	"plot2d(0,0,[0],''012'',''leg'',rect);"
	"a=gca();t=a.title;"
	"t.text=""set of rectangles""; t.font_size=3"
	"xclip(''clipgrf'');"
	"x=[-1 1 1 -1 -1]'';y=[-1 -1 1 1 -1]'';"
	"xx=x*(n-1);yy=y*(n-1);"
	"for k=2:n,;"
	"  xx=[xx,x*((n-k))]; yy=[yy,y*(n-k)];"
	"end;"
	"c=0:(n-1);"
	"xfpolys(xx,yy,c);xclip();"];
	x_message(["[V] set of rectangles";expr]);
	execstr(expr);
	