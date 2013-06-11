// Non-regression test file for bug 2162
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : October 19 2006

mode(-1);
clear;

xf = [0;0;0;0] ;
yf = [0;1;1;0] ;
zf = [0;0;1;1] ; // coordinates of a square

// plot3d routine should be able to draw a single square
plot3d( xf, yf, zf ) ;

e = gce() ;

if ( e.type == "Fac3d" ) then
    affich_result(%T,2162);
else
    affich_result(%F,2162);
end

clear;

