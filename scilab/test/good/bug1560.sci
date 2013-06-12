// Non-regression test file for bug 1560
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : 11 janvier 2006

mode(-1);
clear;

hf = scf (1);
ha = hf.children;
ha.axes_visible = "on";
ha.view = "3d";
ha.box = "on";

xsegs ([0.0,0.0], [1.0,1.0]);
hs10 = gce ();
hs10.data (1,1) = 1.00;  // x1
hs10.data (1,2) = 0.60;  // y1
hs10.data (2,1) = 0.70;  // x2
hs10.data (2,2) = 0.45;  // y2
hs10.data,

ha.rotation_angles = [0,270];
hs10.data (1,3) =-0.30;  // z1
hs10.data (2,3) =-0.40;  // z2
hs10.data,

ha.rotation_angles = [0,0];

reference = [ 1, 0.6, -0.3 ; 0.7, 0.45, -0.4 ] ;

// check that the segment z coordinates are modified
if ( hs10.data == reference ) then
    affich_result(%T,1560);
else
    affich_result(%F,1560);
end

clear;

