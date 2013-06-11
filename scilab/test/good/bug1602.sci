// Non-regression test file for bug 1602
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : 11 janvier 2006

mode(-1);
clear;
clf();

x = linspace(-0.95,0.95,80);
z = x'*x;  // z(x,y) = x*y in fact
clf()
contour2d(x,x,z,8,frameflag=4);
a = gca();

data_boundsRef = [ -0.95, -0.95 ; 0.95, 0.95 ] ;

// check that xfarc fill the arc and don't draw the limit
if ( a.data_bounds == data_boundsRef ) then
    affich_result(%T,1602);
else
    affich_result(%F,1602);
end

clear;

