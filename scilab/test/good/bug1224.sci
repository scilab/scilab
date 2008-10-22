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
