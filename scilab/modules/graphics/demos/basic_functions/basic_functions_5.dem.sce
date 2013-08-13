// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_basic_function5()

    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("basic_functions_5.dem.sce");

    my_plot_desc          = "set of rectangles";
    my_handle.figure_name = my_plot_desc;

    // Demo V
    expr=[
    "n=16;"
    "rect=[-n,-n,n,n];"
    "plot2d(0,0,[0],''012'',''leg'',rect);"
    "a=gca();t=a.title;"
    "t.text=""set of rectangles""; t.font_size=3"
    "a.clip_state = ''clipgrf'';"
    "x=[-1 1 1 -1 -1]'';y=[-1 -1 1 1 -1]'';"
    "xx=x*(n-1);yy=y*(n-1);"
    "for k=2:n,;"
    "  xx=[xx,x*((n-k))]; yy=[yy,y*(n-k)];"
    "end;"
    "c=0:(n-1);"
    "xfpolys(xx,yy,c);"
    "a.clip_state = ''off'';"];

    execstr(expr);

endfunction

demo_basic_function5();
clear demo_basic_function5;
