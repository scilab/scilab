// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2015 - Scilab Enterprises - Juergen Koch <juergen.koch@hs-esslingen.de>
// Copyright (C) 2018-2020 - Samuel GOUGEON
//
// This file is distributed under the same license as the Scilab package.
// ===================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// Test data
t = 1:200;
x = t.*cos(t).*sin(t);
y = t.*sin(t).*sin(t);
z = t.*cos(t);
s = linspace(50,1,length(t));

//
// Test #1 Create 3D scatter plot
clf reset
p = scatter3d(x,y,z);
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
clf reset
p = scatter3d(x,y,z,s);
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
clf reset
set(gcf(),"color_map",coolcolormap(64));
p = scatter3d(x,y,z,s,t);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));  //
assert_checkequal(p.mark_foreground, round((64-1)*(t-min(t))/(max(t)-min(t))+1)); //
assert_checkequal(p.mark_background,0);
set(gca(),"rotation_angles",[60,45]);

//
// Test data
z = linspace(0,25,200);
x = z.*cos(z);
y = z.*sin(z);

//
// Test #4 Fill the markers
clf reset
p = scatter3d(x,y,z,"fill");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,2);

//
// Test #5 Specify marker symbol
clf reset
p = scatter3d(x,y,z,"*");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,10);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);

//
// Test #6 Change marker edge and face color
clf reset
p = scatter3d(x,y,z,...
         "markerEdgeColor","black",...
         "markerFaceColor",[0 .8 .8]);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,color("black"));
assert_checkequal(p.mark_background,color(0,.8*255,.8*255));

//
// Test #7 Specify subplot for scatter plot
clf reset
[x,y] = meshgrid(linspace(-2,2,20));
z = exp(-x.^2-y.^2);
subplot(2,1,1);
p1 = scatter3d(gca(),x(:),y(:),z(:));
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
p2 = scatter3d(gca(),x(:),y(:),z(:),"markerFaceColor",[0 .8 .8]);
assert_checkequal(p2.line_mode,"off");
assert_checkequal(p2.fill_mode,"off");
assert_checkequal(p2.thickness,1);
assert_checkequal(p2.mark_mode,"on");
assert_checkequal(p2.mark_style,9);
assert_checkequal(p2.mark_size_unit,"point");
assert_checkequal(p2.mark_size,7);
assert_checkequal(p2.mark_foreground,2);
assert_checkequal(p2.mark_background,color(0,.8*255,.8*255));

//
// Test data
t = 1:200;
x = t.*cos(t).*sin(t);
y = t.*sin(t).*sin(t);
z = t.*cos(t);
s = linspace(50,1,length(t));

//
// Test #8 Modify scatter plot after creation
clf reset
p = scatter3d(x,y,z,s);
p.mark_foreground = addcolor([0.5 0 0]);
p.mark_background = addcolor([0.5 0.5 0]);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground,color(0.5*255,0,0));
assert_checkequal(p.mark_background,color(0.5*255,0.5*255,0));


// ==============
// Error messages
// ==============
msg = "scatter3d: Wrong number of input arguments: At least 3 expected.";
assert_checkerror("scatter3d(1,2)", msg);
msg = "scatter3d: Wrong number of input arguments: At least 4 expected.";
assert_checkerror("scatter3d(gca(),1,2)", msg);
msg = "scatter3d: Argument #1: Wrong type of graphic handle: ''Axes'' expected.";
assert_checkerror("scatter3d(gcf(),1,2,3)", msg);
msg = "scatter3d: Argument #1: Decimal number(s) expected.";
assert_checkerror("scatter3d(int8(1),2,3)", msg);
assert_checkerror("scatter3d(1+%i,2,3)", msg);
msg = "scatter3d: Argument #2: Decimal number(s) expected.";
assert_checkerror("scatter3d(1,int8(2),3)", msg);
assert_checkerror("scatter3d(1,2-%i,3)", msg);
msg = "scatter3d: Arguments #1 and #2: Same numbers of elements expected.";
assert_checkerror("scatter3d(1,[2 3],3)", msg);
msg = "scatter3d: Argument #3: Decimal number(s) expected.";
assert_checkerror("scatter3d(1,2,int8(3))", msg);
assert_checkerror("scatter3d(1,2,%i)", msg);
assert_checkerror("scatter3d(1,2,%z)", msg);
msg = "scatter3d: Arguments #1 and #3: Same numbers of elements expected.";
assert_checkerror("scatter3d(1, 2, [2 3])", msg);

msg = "scatter3d: Argument #4: Decimal number(s) expected.";
assert_checkerror("scatter3d(1,2,3,%i)", msg);

// ==========
// Properties
// ==========
// Accepted / forbidden syntaxes or values
// ---------------------------------------
// Marks styles (case insensitive)
prop = ["marker" "MarKeR" "markerstyle"  "MARKSTYLE"];
markersT = ["." "+" "x" "o" "o+" "circle plus" "d" "fd" "filled diamond" ..
"d+" "diamond plus" "^" "v" ">" "<" "*" "*5" "p" "pentagram" "s" "square"];
    // Markers Numerical codes styles (available)
markersN = [ 0   1   2   9   3        3         5   4         4 ..
  8       8          6   7   12  13  10  14  14      14      11    11];
x = rand(1,20);
y = rand(1,20);
z = rand(1,20);
for p = prop
    for v = markersT
        assert_checkequal(execstr("clf, scatter3d(x,y,z,p,v);", "errcatch"), 0);
        sleep(50)
    end
    for v = unique(markersN)
        assert_checkequal(execstr("clf, scatter3d(x,y,z,p,v);", "errcatch"), 0);
        sleep(50)
    end
end
msg = "scatter3d: Argument #5: Wrong marker specification.";
assert_checkerror("scatter3d(1,2,3,""marker"",""ab"")", msg);
assert_checkerror("scatter3d(1,2,3,""marker"", 20)", msg);
msg = "scatter3d: Argument #5: Scalar (1 element) expected.";
assert_checkerror("scatter3d(1,2,3,""marker"",[1 2])", msg);

msg = "scatter3d: Argument #5: Scalar (1 element) expected.";
assert_checkerror("scatter3d(1,2,3,""linewidth"",[1 2])", msg);
assert_checkerror("scatter3d(1,2,3,""thickness"",[1 2])", msg);
msg = "scatter3d: Argument #5: Non-negative integers expected.";
assert_checkerror("scatter3d(1,2,3,""linewidth"", -1)", msg);
assert_checkerror("scatter3d(1,2,3,""thickness"", -1)", msg);
assert_checkerror("scatter3d(1,2,3,""linewidth"", %i)", msg);
assert_checkerror("scatter3d(1,2,3,""thickness"", %i)", msg);
assert_checkerror("scatter3d(1,2,3,""linewidth"", %z)", msg);
assert_checkerror("scatter3d(1,2,3,""thickness"", %z)", msg);

prop = ["markeredgecolor" "markerforeground" ..
        "markerfacecolor" "markerbackground"];
for p = prop
    assert_checkequal(execstr("clf, scatter3d(1,2,3, p,""orange"")", "errcatch"),0);
    assert_checkequal(execstr("clf, scatter3d(1,2,3, p,""#00FF00"")", "errcatch"),0);
    assert_checkequal(execstr("clf, scatter3d(1,2,3, p, color(""red""))", "errcatch"),0);
end
msg = "scatter3d: Argument #4: Value missing for the given property.";
assert_checkerror("scatter3d(1,2,3,""marker"")", msg);
msg = "scatter3d: Argument #4: Wrong value. Please check allowed syntaxes and values.";
assert_checkerror("scatter3d(1,2,3,""orange"",""AbCd"",[1 2])", msg);
