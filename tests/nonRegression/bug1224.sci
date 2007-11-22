// <-- Non-regression test for bug 1224 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1224
//
// <-- Short Description -->
//    Product of 2x2 matrices produces wrong result.
//    inf=%inf;
//    A=diag([-inf,inf]);
//    A*A  //This is correct
//    A*A*A  //This is wrong


// Non-regression test file for bug 1224
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

inf=%inf;
A=diag([-inf,inf]);
B = A*A*A;

if isnan(B) then
	affich_result(%T,1224);
else
	affich_result(%F,1224);
end
