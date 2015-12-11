// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_basic_function3()
    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("basic_functions_3.dem.sce");

    my_plot_desc          = "Setting graphic context values";
    my_handle.figure_name = my_plot_desc;

    plot2d([-100,500],[-100,600],[-1,-1],"022");

    a           = gca();
    t           = a.title;
    t.text      = my_plot_desc;
    t.font_size = 3;

    // clipping
    a.clip_state = "clipgrf";

    deff("[v]=transl(x,t)","v=x+t*ones(x)");

    messagebox(["[III] Setting graphic context values"],"modal");

    // Demo III.1
    expr=["x=0:0.2:2*%pi;";
    "x1=[sin(x);10*sin(x)];";
    "y1=[cos(x);10*cos(x)];";
    "xsegs(10*x1+200*ones(x1),10*y1+200*ones(y1));"];
    messagebox(["[III.1] segments";expr],"modal");
    execstr(expr);

    // Demo III.2
    expr=["xrect(150,460,100,150);";
    "a.clip_box = [150,460,100,150];";
    "x=0:0.2:2*%pi;";
    "x1=[sin(x);10*sin(x)];";
    "y1=[cos(x);10*cos(x)];";
    "y1=transl(y1,20);";
    "xsegs(10*x1+200*ones(x1),10*y1+200*ones(y1));";
    "a.clip_state = ''clipgrf'';";];
    messagebox(["[III.2] segments+clipping zone";expr],"modal");
    execstr(expr);

    // Demo III.3
    expr=["xset(''line style'',1);";
    "xget(''line style'',1);";
    "xrect(140,470,120,170);"];
    messagebox(["[III.3] Setting a dash style";expr],"modal");
    execstr(expr);

    // Demo III.4
    expr=["xset(''font'',2,0);";
    "xstring(0,130,''Scilab'');";
    "xset(''font'',2,1);";
    "xstring(0,150,''Scilab'');";
    "xset(''font'',2,2);";
    "xstring(0,170,''Scilab'');";
    "xset(''font'',2,3);";
    "xstring(0,200,''Scilab'');";
    "xset(''font'',2,4);";
    "xstring(0,230,''Scilab'');";
    "xset(''font'',2,5);";
    "xstring(0,270,''Scilab'');";
    "xset(''font'',3,5);";
    "xstring(0,310,''Scilab'');";
    "xset(''font'',4,5);";
    "xstring(0,350,''Scilab'');"]
    messagebox(["[III.4] Setting font style and size";expr],"modal");
    execstr(expr);

    // Demo III.5
    expr=["xset(''line mode'',0);";
    "xlfont(''SansSerif'',6);";
    "xset(''font'',6,3);";
    "xstring(100,260,''helVB'',0,1);"];
    messagebox(["[III.5] Loading a new font hlevB is now font 6";expr],"modal");
    execstr(expr);

    // Demo III.6
    expr0=["x1=[350,400,450];";
    "[n1,n2]=size(x1);";
    "y1=300*ones(x1);"];
    expr=[expr0;
    "xset(''mark'',6,2);";
    "xpoly(x1,y1,''marks'',0);"];
    messagebox(["[III.6] changing current mark";expr],"modal");
    execstr(expr);

    // Demo III.7
    expr=[expr0;
    "xset(''mark'',7,2);";
    "xpoly(x1,y1+50*ones(x1),''marks'',0);";
    "xset(''mark'',8,3);";
    "xpoly(x1,y1+100*ones(x1),''marks'',0);"];
    messagebox(["[III.7] changing current mark";expr],"modal");
    execstr(expr);

    // Demo III.8
    expr=["xset(''thickness'',5);";
    "xrect(400,200,50,100);";];
    messagebox(["[III.8] Thickness)";expr],"modal");
    execstr(expr);

    // Demo III.9
    expr=["xfrect(400,50,50,100);"];
    messagebox(["[III.9] Clear a rectangle";expr],"modal");
    execstr(expr);

endfunction

demo_basic_function3();
clear demo_basic_function3;
