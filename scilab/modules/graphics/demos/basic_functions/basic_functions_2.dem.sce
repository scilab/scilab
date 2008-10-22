//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

mode(-1);

my_handle             = scf(100001);
clf(my_handle,"reset");
demo_viewCode("basic_functions_2.dem.sce");

my_plot_desc          = "rectangles, arcs, polylines and strings";
my_handle.figure_name = my_plot_desc;

plot2d([-100,500],[-100,600],[-1,-1],"022");
a           = gca();
t           = a.title;
t.text      = my_plot_desc;
t.font_size = 3;

// clipping
a.clip_state = "clipgrf";

deff('[v]=transl(x,t)','v=x+t*ones(x)');

x_message(["[II] Basic Functions of drivers :"]);

// Demo II.1
expr="xrect(20,120,60,60);";
x_message(["[II.1] xrect : draw one rectangle with current line style";expr]);
execstr(expr);

// Demo II.2
expr="xfrect(100,120,60,60);";
x_message(["[II.1] xfrect : fill one rectangle with current pattern";expr]);
execstr(expr);

// Demo II.3
expr="xarc(20,200,50,70,0,64*(225));";
x_message(["[II.2] xarc : draw part of an elipsis";expr]);
execstr(expr);

// Demo II.4
expr="xfarc(100,200,50,70,0,64*225);";
x_message(["[II.2] xfarc : fill part of an elipsis";expr]);
execstr(expr);

// Demo II.5
expr0=["x=0:1:%pi;";
"[n1,n2]=size(x);";
"x1=50*sin(x)+40*ones(x);";
"y1=50*cos(x)+90*ones(x);"];

expr=[expr0;
"xset(''mark'',6,1);";
"y1=transl(y1,200);";
"xpoly(x1,y1,''marks'',0);";
"xset(''mark'',4,6);";
"x1=transl(x1,80);";
"xpoly(x1,y1,''marks'',0);"];

x_message(["[II.3] xpoly(x,y,""marks"",0)";expr]);
execstr(expr);

// Demo II.6
expr=[expr0;"x1=transl(x1,160);";
"xpoly(x1,y1,''lines'',0);"];
x_message(["[II.3] xpoly(x,y,""lines"",0)";expr]);
execstr(expr);

// Demo II.7
expr=[expr0;"x1=transl(x1,240);";
"xpoly(x1,y1,''lines'',1);"]
x_message(["[II.3] xpoly(x,y,""lines"",1)";expr]);
execstr(expr);

// Demo II.8
expr=[expr0;"x1=transl(x1,320);";
"xfpoly(x1,y1,1);"]
x_message(["[II.4] xfpoly(x,y,1)";expr]);
execstr(expr);

// Demo II.9
expr=["xset(''font'',2,2);";
"xstring(200,200,''Character string'',0,0);";
"xstring(200,250,''Character string'',0,1);";
"xstring(400,300,''Chaine '',45,0);"];
x_message(["[II.5] xstring";expr]);
execstr(expr);

// Demo II.10
expr=["rect=xstringl(350,200,''Character string'');";
"xrect(rect(1),rect(2),rect(3),rect(4));"]
x_message(["[II.6] xstringl";expr]);
execstr(expr);
