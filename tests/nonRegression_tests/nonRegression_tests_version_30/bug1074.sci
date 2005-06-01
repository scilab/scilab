// Non-regression test file for bug 1057
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

function foo()
	[a,b,c]=foo1(1)
endfunction

A = fun2string(foo)
B = ["function []=ans";"  [a,b,c] = foo1(1)";"endfunction"]

if and( A == B ) then
	affich_result(%T,1074);
else
	affich_result(%F,1074);
end
