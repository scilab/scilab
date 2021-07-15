// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015-2010 - Scilab Enterprises - Juergen Koch <juergen.koch@hs-esslingen.de>
// Copyright (C) 2018-2020 - Samuel GOUGEON
//
// This file is distributed under the same license as the Scilab package.
// ===================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// test data
rand("seed",0);
x = linspace(0,2,200);
y = exp(-x).*cos(10*x) + 0.2*rand(1,length(x));
s = linspace(1,30,length(x));

clf reset

//
// Test #1 Create 2D scatter plot
p = scatter(x,y);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);
//
// Test #2 Vary marker size
clf
p = scatter(x,y,s);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);
//
// Test #3 Vary marker size and color
clf
set(gcf(),"color_map",coolcolormap(64))
p = scatter(x,y,s,x);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground, round((64-1)*(x-min(x))/(max(x)-min(x))+1));
assert_checkequal(p.mark_background, 0);
//
// Test #4 Fill the markers
clf reset
p = scatter(x,y,s,y,"fill");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground, round((32-1)*(y-min(y))/(max(y)-min(y))+1));
assert_checkequal(p.mark_background, round((32-1)*(y-min(y))/(max(y)-min(y))+1));
//
// Test #5 Specify subplot for scatter plot
clf
subplot(2,1,1);
p1 = scatter(gca(),x,y);
assert_checkequal(p1.line_mode,"off");
assert_checkequal(p1.fill_mode,"off");
assert_checkequal(p1.thickness,1);
assert_checkequal(p1.mark_mode,"on");
assert_checkequal(p1.mark_style,9);
assert_checkequal(p1.mark_size_unit,"point");
assert_checkequal(p1.mark_size,7);
assert_checkequal(p1.mark_foreground,2);
assert_checkequal(p1.mark_background,0);
subplot(2,1,2);
p2 = scatter(gca(),x,y,"fill","s");  // msizes & mcolors are implicitly skipped !
assert_checkequal(p2.line_mode,"off");
assert_checkequal(p2.fill_mode,"off");
assert_checkequal(p2.thickness,1);
assert_checkequal(p2.mark_mode,"on");
assert_checkequal(p2.mark_style,11);
assert_checkequal(p2.mark_size_unit,"point");
assert_checkequal(p2.mark_size,7);
assert_checkequal(p2.mark_foreground,2);
assert_checkequal(p2.mark_background,2);

//
// test data
rand("seed",0);
x = rand(1,200);
y = rand(1,200);

//
// Test #6 Specify marker symbol
clf
p = scatter(x,y,"d");   // msizes, mcolors and "fill" are implicitly skipped !
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,5);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);
//
// Test #7 Change marker color and line width
clf
p = scatter(x,y,"markerEdgeColor",[0 .4 .4],...
                "markerFaceColor",[0 .8 .8],...
                "linewidth",1.5);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1.5);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,color(0,.4*255,.4*255));
assert_checkequal(p.mark_background,color(0,.8*255,.8*255));

//
// Test data
t = linspace(0,25,200);
x = t.*cos(t);
y = t.*sin(t);

//
// Test #8 Modify scatter plot after creation
clf
p = scatter(x,y,t);
p.thickness = 0.5;
p.mark_foreground = color("darkblue");
p.mark_background = color("darkcyan");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,0.5);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*t/%pi)));
assert_checkequal(p.mark_foreground,color("darkblue"));
assert_checkequal(p.mark_background,color("darkcyan"));
//
// Test #9 Testing graphics handle
clf
p = scatter(gca(),x,y,t);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*t/%pi)));
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);

//
// Test data
x = 0.1:0.2:0.9;

//
// Test #10 Testing single color string specification
clf
p = scatter(x,x,[],"red");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,color("red"));
assert_checkequal(p.mark_background,0);
//
// Test #11 Testing color vector of strings specification
clf
p = scatter(x,x,[],["red" "green" "blue" "grey" "black"]);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,[color("red") color("green") color("blue") color("grey") color("black")]);
assert_checkequal(p.mark_background,0);
//
// Test #12 Testing color RGB matrix specification
clf
RGB = [255   0   0
         0 255   0
         0   0 255
       128 128 128
         0   0   0];
p = scatter(x,x,[],RGB/255);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,...
[color(RGB(1,1),RGB(1,2),RGB(1,3))...
 color(RGB(2,1),RGB(2,2),RGB(2,3))...
 color(RGB(3,1),RGB(3,2),RGB(3,3))...
 color(RGB(4,1),RGB(4,2),RGB(4,3))...
 color(RGB(5,1),RGB(5,2),RGB(5,3))]);
assert_checkequal(p.mark_background,0);

// ==============
// Error messages
// ==============
msg = "scatter: Wrong number of input arguments: At least 2 expected.";
assert_checkerror("scatter(1)", msg);
msg = "scatter: Wrong number of input arguments: At least 3 expected.";
assert_checkerror("scatter(gca(),1)", msg);
msg = "scatter: Argument #1: Wrong type of graphic handle: ''Axes'' expected.";
assert_checkerror("scatter(gcf(),1,2)", msg);
msg = "scatter: Argument #1: Decimal number(s) expected.";
assert_checkerror("scatter(int8(1),2)", msg);
assert_checkerror("scatter(1+%i,2)", msg);
msg = "scatter: Argument #2: Decimal number(s) expected.";
assert_checkerror("scatter(1,int8(2))", msg);
assert_checkerror("scatter(1,2-%i)", msg);
assert_checkerror("scatter(1,%z)", msg);
msg = "scatter: Arguments #1 and #2: Same numbers of elements expected.";
assert_checkerror("scatter(1,[2 3])", msg);

msg = "scatter: Argument #3: Decimal number(s) expected.";
assert_checkerror("scatter(1,2,%i)", msg);
msg = "scatter: Argument #3: Wrong value. Please check allowed syntaxes and values.";
assert_checkerror("scatter(1,2,""A"")", msg);
msg = "scatter: Arguments #1 and #3: Same numbers of elements expected.";
assert_checkerror("scatter(1,2,[1 2])", msg);
msg = "scatter: Argument #4: Wrong size: one or 1 colors expected.";
assert_checkerror("scatter(1,2,3,[1 2])", msg);
assert_checkerror("scatter(1,2,3,[""cyan"" ""red""])", msg);

msg = "scatter: Argument #4: Decimal number(s) expected.";
assert_checkerror("scatter(1,2,3,%i)", msg);

// ==========
// Properties
// ==========
// Marks styles (case insensitive)
prop = ["marker" "MarKeR" "markerstyle"  "MARKSTYLE"];
markersT = ["." "+" "x" "o" "o+" "circle plus" "d" "fd" "filled diamond" ..
"d+" "diamond plus" "^" "v" ">" "<" "*" "*5" "p" "pentagram" "s" "square"];
    // Markers Numerical codes styles (available)
markersN = [ 0   1   2   9   3        3         5   4         4 ..
  8       8          6   7   12  13  10  14  14      14      11    11];
x = rand(1,20);
y = rand(1,20);
for p = prop
    for v = markersT
        assert_checkequal(execstr("clf, scatter(x,y,p,v);", "errcatch"), 0);
        sleep(50)
    end
    for v = unique(markersN)
        assert_checkequal(execstr("clf, scatter(x,y,p,v);", "errcatch"), 0);
        sleep(50)
    end
end
msg = "scatter: Argument #4: Wrong marker specification.";
assert_checkerror("scatter(1,2,""marker"",""ab"")", msg);
assert_checkerror("scatter(1,2,""marker"", 20)", msg);
msg = "scatter: Argument #4: Scalar (1 element) expected.";
assert_checkerror("scatter(1,2,""marker"",[1 2])", msg);
msg = "scatter: Argument #5: Unknown property name ''A''.";
assert_checkerror("scatter(1,2,10,4,""A"")", msg);

msg = "scatter: Argument #4: Scalar (1 element) expected.";
assert_checkerror("scatter(1,2,""linewidth"",[1 2])", msg);
assert_checkerror("scatter(1,2,""thickness"",[1 2])", msg);
msg = "scatter: Argument #4: Non-negative integers expected.";
assert_checkerror("scatter(1,2,""linewidth"", -1)", msg);
assert_checkerror("scatter(1,2,""thickness"", -1)", msg);
assert_checkerror("scatter(1,2,""linewidth"", %i)", msg);
assert_checkerror("scatter(1,2,""thickness"", %i)", msg);
assert_checkerror("scatter(1,2,""linewidth"", %z)", msg);
assert_checkerror("scatter(1,2,""thickness"", %z)", msg);

prop = ["markeredgecolor" "markerforeground" ..
        "markerfacecolor" "markerbackground"];
for p = prop
    assert_checkequal(execstr("clf, scatter(1,2, p,""orange"")", "errcatch"),0);
    assert_checkequal(execstr("clf, scatter(1,2, p,""#00FF00"")", "errcatch"),0);
    assert_checkequal(execstr("clf, scatter(1,2, p, color(""red""))", "errcatch"),0);
end
msg = "scatter: Argument #3: Value missing for the given property.";
assert_checkerror("scatter(1,2,""marker"")", msg);
msg = "scatter: Argument #3: Wrong value. Please check allowed syntaxes and values.";
assert_checkerror("scatter(1,2,""orange"",""AbCd"",[1 2])", msg);
