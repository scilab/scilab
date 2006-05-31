// Non-regression test file for bug 1057
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

if 1.7977E+308 == %inf  then
	affich_result(%T,1057);
else
	affich_result(%F,1057);
end
