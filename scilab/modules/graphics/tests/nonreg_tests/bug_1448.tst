// <-- TEST WITH GRAPHIC -->

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

// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : October 20 2006

point1 = [1 ; 0];
point2 = rotate (point1, %pi/2),	

if norm( point2 - [0;1] ) >= 10e-10 then pause,end
