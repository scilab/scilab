// Non-regression test file for bug 1682
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : 11 janvier 2006

mode(-1);
clear;

//  Program Spas42151A
//  Designed for Scilab 3.1 & 4.0
//  Copyright Jacques-Deric Rouault, CNRS, INRIA

disp ("4.21.51A");
hf = scf (1);
hf.figure_name = "4.21.51A";
hf.background = 32;
hf.rotation_style = "multiple";

subplot (1, 2, 1);
xfrect (0, 1, 1, 1);
hr1 = gce ();
hr1.background = 2;
hr1.fill_mode = "on";
ha1 = hr1.parent; // get the axes

subplot (1, 2, 2);
xfrect (0, 1, 1, 1);
hr2 = gce ();
hr2.background = 3;
hr2.fill_mode = "on";
ha2=hr2.parent; // get the axes

newAngles = [60,200] ;
ha1.rotation_angles = newAngles ;

// check if both axes are turned
if ( (ha1.rotation_angles == newAngles) & (ha2.rotation_angles == newAngles) ) then
    affich_result(%T,1682);
else
    affich_result(%F,1682);
end

clear;

