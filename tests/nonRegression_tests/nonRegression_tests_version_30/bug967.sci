// Non-regression test file for bug 967
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

m=[5 %inf;2 %i];

if ~or(m==%nan) then
	affich_result(%T,967);
else
	affich_result(%F,967);
end
