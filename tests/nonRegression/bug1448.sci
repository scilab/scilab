// <-- Non-regression test for bug 1448 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1448
//
// <-- Short Description -->
//    Bug detected in 3.1.1 under Windows XP.
//    Bug relative to function rotate
//
//    A positive angle corresponds to a clockwise rotation, then in the 
//    negative mathematical way.
//
//    Jacques-Deric


// Non-regression test file for bug 1448
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : October 20 2006

mode(-1);
clear;

point1 = [1 ; 0];
point2 = rotate (point1, %pi/2),	


if ( norm( point2 - [0;1] ) < 10e-10 ) then
  affich_result(%T,1448);
else
  affich_result(%F,1448);
end

clear;

