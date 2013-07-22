// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_basic_function1()

    mode(-1);

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("basic_functions_1.dem.sce");

    my_plot_desc          = "rectangles, arcs, polylines, segments and numbers";
    my_handle.figure_name = my_plot_desc;
    colorMapSize = size(my_handle.color_map,1);

    plot2d([-100,500],[-100,600],[-1,-1],"022");

    a            = gca();
    t            = a.title;
    t.text       = my_plot_desc;
    t.font_size  = 3;

    // clipping
    a.clip_state = "clipgrf";


    deff("[v]=transl(x,t)","v=x+t*ones(x)");

    messagebox(["[I] function to plot a set of objects "],"modal");

    // Demo I.1
    expr=["x=0:40:240;";
    "boxes=[x;10*ones(x);30*ones(x);30*ones(x)];";
    "xrects(boxes)"];
    messagebox(["[I.1] function xrects : plot rectangles";expr],"modal");
    execstr(expr);

    // Demo I.2
    expr=["x=0:40:240;";
    "boxes=[x;45*ones(x);30*ones(x);30*ones(x)];";
    "pats=[0,4,8,12,15,colorMapSize,0];";
    "xrects(boxes,pats);"];
    messagebox(["[I.2] fonction xrects : filling rectangles";expr],"modal");
    execstr(expr);

    // Demo I.3
    expr=["x=0:40:240;";
    "boxes=[x;90*ones(x);30*ones(x);30*ones(x)];";
    "arcs=[boxes; 0*ones(x);64*180*ones(x)];";
    "pats=[0,4,8,12,15,colorMapSize,0];";
    "xarcs(arcs,pats);"];
    messagebox(["[I.3] xarcs(arcs,patterns)";expr],"modal");
    execstr(expr);

    // Demo I.4
    expr=["x=0:40:240;";
    "boxes=[x;135*ones(x);30*ones(x);30*ones(x)];";
    "arcs=[boxes; 0*ones(x);64*360*ones(x)];";
    "xarcs(arcs);"];
    messagebox(["[I.4] xarcs(arcs)";expr],"modal");
    execstr(expr);

    // Demo I.5
    expr0=["x1=[0,10,20,30,20,10,0];";
    "y1=[15,30,30,15,0,0,15];y1=160*ones(y1)+y1;";
    "xpols=[x1;transl(x1,40);transl(x1,80);transl(x1,120)"...
    + ";transl(x1,160);transl(x1,200);transl(x1,240)];";
    "ypols=[y1;y1;y1;y1;y1;y1;y1];";
    "xfpolys(xpols'',ypols'');"];
    messagebox(["[I.5] xfpolys(x,y)";expr0],"modal");
    execstr(expr0);

    // Demo I.6
    expr=[ expr0;
    "ypols=transl(ypols,60);";
    "pats=[0,4,8,12,15,colorMapSize,0];";
    "xfpolys(xpols'',ypols'',pats);"];
    messagebox(["[I.6] xfpolys(x,y,pattern)";expr],"modal");
    execstr(expr);

    // Demo I.7
    expr=[ expr0;
    "ypols=transl(ypols,120);";
    "xpolys(xpols'',ypols'',1:7);"];
    messagebox(["[I.7] xpolys(x,y,line-style)";expr],"modal");
    execstr(expr);

    // Demo I.8
    expr=[ expr0;
    "ypols=transl(ypols,180);";
    "xpolys(xpols'',ypols'',-(1:7));"];
    messagebox(["[I.8] xpolys(x,y,line-style)";expr],"modal");
    execstr(expr);

    // Demo I.9
    expr=["x=0:40:240;";
    "xsegs([x;x+30*ones(x)],[(360+40)*ones(x);(360+70)*ones(x)]);"];
    messagebox(["[I.9] xsegs(x,y)";expr],"modal");
    execstr(expr);

    // Demo I.10
    expr=["x=0:40:240;";
    "xarrows([x;x+30*ones(x)],[(360+70)*ones(x);(360+100)*ones(x)]);"];
    messagebox(["[I.10] xarrows(x,y)";expr],"modal");
    execstr(expr);

    // Demo I.11
    expr=["x=0:100:200;";
    "xnumb(x,500*ones(x),[10,20,35],1);";
    "xnumb(x,550*ones(x),[10,20,35],0);";];
    messagebox(["[I.10] xnumb()";expr],"modal");
    execstr(expr);

endfunction


demo_basic_function1();
clear demo_basic_function1;


