// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_basic_function7()

    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("basic_functions_7.dem.sce");

    my_plot_desc          = "colored histogram";
    my_handle.figure_name = my_plot_desc;

    expr=[
    "plot2d(0,0,[0],''012'',''leg'',[0,0,30,20]);"
    "a=gca(); t=a.title;"
    "t.text=""colored histogram""; t.font_size=3;"
    "a.clip_state = ''clipgrf'';"
    "x=[1 3 3 1 1]'';"
    "y=[0 0 1 1 0]'';"
    "d=4*[1 1 1 1 1]'';"
    "xx=[x,x+d,x+2*d,x+3*d,x+4*d,x+5*d];"
    "yy=[y,5*y,2*y,10*y,8*y,6*y];"
    "xfpolys(xx,yy,2*[1 2 3 4 5 6]);a.clip_state = ''off'';"];

    execstr(expr);

endfunction

demo_basic_function7();
clear demo_basic_function7;
