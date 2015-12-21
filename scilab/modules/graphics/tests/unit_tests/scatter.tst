// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015-2010 - Juergen Koch <juergen.koch@hs-esslingen.de>
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

//
// test data
rand("seed",0);
x = linspace(0,2,200);
y = exp(-x).*cos(10*x) + 0.2*rand(1,length(x));
s = linspace(1,30,length(x));

//
// Test #1 Create 2D scatter plot 
h = scf();
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
close(h);
//
// Test #2 Vary marker size
h = scf();
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
close(h);
//
// Test #3 Vary marker size and color
h = scf();
set(gcf(),"color_map",coolcolormap(64))
p = scatter(x,y,s,x);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground,floor((64-1)*(x-min(x))/(max(x)-min(x))+1));
assert_checkequal(p.mark_background,0);
close(h);
//
// Test #4 Fill the markers
h = scf();
p = scatter(x,y,s,y,"fill");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground,floor((32-1)*(y-min(y))/(max(y)-min(y))+1));
assert_checkequal(p.mark_background,floor((32-1)*(y-min(y))/(max(y)-min(y))+1));
close(h);
//
// Test #5 Specify subplot for scatter plot
h = scf();
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
p2 = scatter(gca(),x,y,"fill","s");
assert_checkequal(p2.line_mode,"off");
assert_checkequal(p2.fill_mode,"off");
assert_checkequal(p2.thickness,1);
assert_checkequal(p2.mark_mode,"on");
assert_checkequal(p2.mark_style,11);
assert_checkequal(p2.mark_size_unit,"point");
assert_checkequal(p2.mark_size,7);
assert_checkequal(p2.mark_foreground,2);
assert_checkequal(p2.mark_background,2);
close(h);

//
// test data
rand("seed",0);
x = rand(1,200);
y = rand(1,200);

//
// Test #6 Specify marker symbol
h = scf();
p = scatter(x,y,"d");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,5);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);
close(h);
//
// Test #7 Change marker color and line width
h = scf();
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
close(h);

//
// Test data
t = linspace(0,25,200);
x = t.*cos(t);
y = t.*sin(t);

//
// Test #8 Modify scatter plot after creation
h = scf();
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
close(h);
//
// Test #9 Testing graphics handle
h = scf();
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
close(h);

//
// Test data
x = 0.1:0.2:0.9;

//
// Test #10 Testing single color string specification
h = scf();
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
close(h);
//
// Test #11 Testing color vector of strings specification
h = scf();
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
close(h);
//
// Test #12 Testing color RGB matrix specification
h = scf();
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
close(h);
