// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

//
// Test #1
h = scf();
contour2d(1:10,1:10,rand(10,10),5,rect=[0,0,11,11]);
// changing the format of the printing of the levels
xset("fpf","%.2f")
clf()
contour2d(1:10,1:10,rand(10,10),5,rect=[0,0,11,11]);
close(h);
//
// Test #2
// now an example with level numbers drawn in a legend
// Caution there are a number of tricks...
x = linspace(0,4*%pi,80);
z = cos(x')*cos(x);
h = scf();
f=gcf();
xset("fpf"," ")  // trick 1: this implies that the level numbers are not
//          drawn on the level curves
f.color_map=jetcolormap(7);
// trick 2: to be able to put the legend on the right without
//          interfering with the level curves use rect with a xmax
//          value large enough
contour2d(x,x,z,-0.75:0.25:0.75,frameflag=3,rect=[0,0,5*%pi,4*%pi]);
// trick 3: use legends (note that the more practical legend function
//          will not work as soon as one of the level is formed by 2 curves)
legends(string(-0.75:0.25:0.75),1:7,"lr");
xtitle("Some level curves of the function cos(x)cos(y)");
close(h);
