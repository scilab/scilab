// Non-regression test file for bug 2362
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2007
// Date : 5 avril 2007

mode(-1);
clear;

// crashed before the bug fix
plot3d3([0,0;10,10],[5,3;5,5],[0,2;0,2]);

affich_result(%T,2362);

clear;

