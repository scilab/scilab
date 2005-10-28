// Non-regression test file for bug 1568
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 21 octobre 2005

mode(-1);
clear;

grand('setsd',12);
A = grand(1,'prm',[1:5]');
grand('setsd',12);
B = grand(1,'prm',[1:5]');

if or(A<>B) then
	affich_result(%F,1568);
else
	affich_result(%T,1568);
end

clear
