// Non-regression test file for bug 1183
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1)
clear

A =1@1
B=lasterror()

if B=='Missing operator, comma, or semicolon' then
	affich_result(%T,1183);
else
	affich_result(%F,1183);
end
