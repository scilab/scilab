// Non-regression test file for bug 2410
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 05 mai 2007

mode(-1);
clear;

tic();
string(zeros(1,10000)==1);
a = toc();

if( a < 0.1 ) then
	affich_result(%T,2410);
else
	affich_result(%F,2410);
end

clear
