// Non-regression test file for bug 2414
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 05 mai 2007

mode(-1);
clear;

a=[];
a = string(a);

if( (type(a) == 1) & (typeof(a) == "constant") & and(size(a) == [0 0]) & (a($,1)==[]) ) then
	affich_result(%T,2414);
else
	affich_result(%F,2414);
end

clear
