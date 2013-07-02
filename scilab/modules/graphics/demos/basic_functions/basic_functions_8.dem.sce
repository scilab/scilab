// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_basic_function8()

    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("basic_functions_8.dem.sce");

    my_plot_desc          = "pie chart";
    my_handle.figure_name = my_plot_desc;

    expr=[
    "plot2d(0,0,[0],''032'',''leg'',[-1.5,-1.5 ,1.5,1.5]);"
    "a=gca(); t=a.title;"
    "t.text=""pie chart""; t.font_size=3;"
    "a.clip_state = ''clipgrf'';"
    "alls=[-1,1,2,2,0,64*90;"
    "    -1,1,2,2,64*90,64*(30);"
    "    -1,1,2,2,64*(120),64*(70);"
    "    -1,1,2,2,64*(190),64*(360-190)];"
    "xfarcs(alls'',[1,3,5,7]);a.clip_state = ''off'';"];

    execstr(expr);

endfunction

demo_basic_function8();
clear demo_basic_function8;