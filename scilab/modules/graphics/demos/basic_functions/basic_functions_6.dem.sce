//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
	
	my_plot_desc          = "colored matrix";
	
	my_handle             = scf(100001);
	my_handle.figure_name = my_plot_desc;

	expr=[
	"rect=[0,0,100,100];"
	"plot2d(0,0,[0],''012'',''leg'',rect);"
	"a=gca(); t=a.title;"
	"t.text=""colored matrix""; t.font_size=3;"
	"xclip(''clipgrf'');"
	"x=[0 25 25  0 0]'';y=[0 0 25 25 0]'';d=25*[1 1 1 1 1]'';"
	"xx=[];yy=[];i=0;"
	"for k=1:4"
	"  for l=1:4"
	"    i=i+1;"
	"    xx=[xx,(l-1)*d+x];yy=[yy,y+(k-1)*d];"
	"  end"
	"end"
	"xfpolys(xx,yy,(1:16));xclip();"];
	x_message(["[VI]";expr]);
	execstr(expr);
	