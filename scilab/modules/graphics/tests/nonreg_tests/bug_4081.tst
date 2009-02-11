// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4081 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4081
//
// <-- Short Description -->
// Colormap is sometime not updated when using new interpoated colors.


clf()
f=gcf();
a=gca();a.data_bounds=[0,-10;40,30];
x1=[0,10,20,10]';
y1=[10,0,10,20]';
c=linspace(2,100,4)';
xpols=[x1 x1+20 x1+10 x1+10];
ypols=[y1 y1    y1+10 y1-10];
cols= [c c($:-1:1) c([3 4 1 2]) c]
f.color_map=jetcolormap(max(cols));
xfpolys(xpols,ypols,cols)
// interpolated colors
clf()
f=gcf();
x11=[0;20;20;0];y11=[10;10;30;30];c11=[10;10;30;30];
x12=x11;y12=y11+20;c12=[20;20;1;1];c12=[30;30;10;10];
x21=[0;30;30;0]+22;y21=[20;20;30;30];c21=[20;20;30;30];
x22=x21;y22=y21+10;c22=[30;30;20;20];
x31=[0;40;40;0]+55;y31=[0;0;30;30];c31=[0;0;30;30];
x32=x31;y32=y31+30;c32=[30;30;0;0];
X=[x11 x12 x21 x22 x31 x32];Y=[y11 y12 y21 y22 y31 y32];C=([c11 c12 c21 c22  c31 c32]+1)*5;
a=gca();
a.data_bounds=[min(X),min(Y);max(X),max(Y)];
f=gcf();f.color_map=graycolormap(max(C));
xfpolys(X,Y,C)

// the rectangles should be gray and not using the jetcolormap
