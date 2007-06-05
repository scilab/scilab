// Non-regression test file for bug 2407
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 05 mai 2007

mode(-1);
clear;

a = zeros(3,3,3);
b = stdev(a);

if( b == 0 ) then
	affich_result(%T,2407);
else
	affich_result(%F,2407);
end

clear
