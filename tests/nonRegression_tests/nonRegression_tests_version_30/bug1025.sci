// Non-regression test file for bug 1025
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

set figure_style old;
xset('window',1000);
set figure_style old;
xset('window',0);
set figure_style new;

A = winsid();

if and(A == [0,1000]) then
	affich_result(%T,1025);
else
	affich_result(%F,1025);
end
