// Non-regression test file for bug 1180
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1)
clear

x = zeros(1:10);
x(5) = 1;
b = [ 0 1 0 ]
y = mtlb_filter(b,1,x);

if y==[0,0,0,0,0,1,0,0,0,0] then
	affich_result(%T,1180);
else
	affich_result(%F,1180);
end
